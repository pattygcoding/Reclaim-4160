#include "reclaim/scenes/PlayScene.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "reclaim/core/Assets.hpp"
#include "reclaim/core/Game.hpp"
#include "reclaim/scenes/GameOverScene.hpp"
#include "reclaim/scenes/TitleScene.hpp"
#include "reclaim/scenes/WinScene.hpp"

namespace reclaim {

PlayScene::PlayScene(Game& game, int levelIndex)
    : Scene(game), levelIndex_(levelIndex), particles_(game.rng()) {
    const std::vector<Level> campaign = makeCampaign();
    if (levelIndex_ < 0) levelIndex_ = 0;
    if (levelIndex_ >= static_cast<int>(campaign.size())) levelIndex_ = static_cast<int>(campaign.size()) - 1;
    level_ = campaign[levelIndex_];
    tpGoal_ = level_.tpGoal;
    score_ = game_.session().score;
}

void PlayScene::enter() {
    game_.session().level = levelIndex_ + 1;
    score_ = game_.session().score;
    game_.audio().playMusic(game_.assets().music("music_game"));

    game_.assets().warmup({"player", "shadow", "enemy_patroller", "enemy_chaser",
                           "tp", "sanitizer", "cart", "checkout"});

    pauseMenu_.clear();
    pauseMenu_.add("RESUME");
    pauseMenu_.add("RESTART");
    pauseMenu_.add("HOME");

    buildLevel();
}

void PlayScene::buildLevel() {
    solids_.clear();
    enemies_.clear();
    pickups_.clear();
    carts_.clear();
    particles_.clear();

    cameraX_ = 0.0f;
    shake_ = 0.0f;
    shakeSeed_ = 0.0f;
    elapsed_ = 0.0f;
    hurtFlash_ = 0.0f;
    phase_ = Phase::Playing;
    phaseTimer_ = 0.0f;
    tpCollected_ = 0;
    tpGoal_ = level_.tpGoal;
    checkoutOpen_ = false;
    loseByTime_ = false;
    timeRemaining_ = level_.timeLimit;

    // Floor is one big solid; each crate is an additional solid box.
    solids_.push_back({0.0f, level_.floorY, level_.worldWidth, 400.0f});
    for (const Rect& crate : level_.crates) solids_.push_back(crate);

    player_ = std::make_unique<Player>(game_.assets());
    player_->respawn(level_.playerStart);

    for (const Vec2& p : level_.tpSpawns)
        pickups_.push_back(std::make_unique<Pickup>(game_.assets(), PickupKind::ToiletPaper, p));
    for (const Vec2& p : level_.sanitizerSpawns)
        pickups_.push_back(std::make_unique<Pickup>(game_.assets(), PickupKind::Sanitizer, p));
    for (const Vec2& p : level_.cartSpawns)
        carts_.push_back(std::make_unique<Pushable>(game_.assets(), p));
    for (const EnemySpawn& spawn : level_.enemySpawns)
        enemies_.push_back(std::make_unique<Enemy>(game_.assets(), spawn));

    checkout_ = std::make_unique<Checkout>(game_.assets(), level_.checkoutPos);
}

void PlayScene::update(float dt) {
    const Input& input = game_.input();

    if (phase_ == Phase::Playing && input.justPressed(SDL_SCANCODE_ESCAPE)) {
        phase_ = Phase::Paused;
        game_.audio().playSound(game_.assets().sound("select"));
    } else if (phase_ == Phase::Paused && input.justPressed(SDL_SCANCODE_ESCAPE)) {
        phase_ = Phase::Playing;
    }

    switch (phase_) {
        case Phase::Playing:
            updatePlaying(dt);
            break;

        case Phase::Paused: {
            pauseMenu_.animate(dt);
            const Menu::Result result = pauseMenu_.update(input);
            if (result.moved) game_.audio().playSound(game_.assets().sound("select"));
            if (result.confirmed) {
                game_.audio().playSound(game_.assets().sound("confirm"));
                switch (result.selected) {
                    case 0: phase_ = Phase::Playing; break;
                    case 1: buildLevel(); break;  // restart keeps score already earned
                    default:
                        game_.requestScene(std::make_unique<TitleScene>(game_));
                        break;
                }
            }
            particles_.update(dt);
            break;
        }

        case Phase::Winning:
        case Phase::Losing:
            handleTransitions(dt);
            break;
    }
}

void PlayScene::updatePlaying(float dt) {
    elapsed_ += dt;
    hurtFlash_ = damp(hurtFlash_, 0.0f, 4.0f, dt);

    // Player shoving carts: nudge any cart the player is overlapping away from them.
    for (auto& cart : carts_) {
        if (player_->bounds().intersects(cart->bounds())) {
            const float dir = sign(cart->center().x - player_->center().x);
            if (player_->velocity.x * dir > 0.0f) cart->push(dir, dt, *this);
        }
    }

    player_->update(dt, *this);
    for (auto& enemy : enemies_) enemy->update(dt, *this);
    for (auto& cart : carts_) cart->update(dt, *this);
    for (auto& pickup : pickups_) pickup->update(dt, *this);

    resolvePlayerInteractions();

    checkout_->update(dt, *this);
    particles_.update(dt);
    updateCamera(dt);

    // Prune finished entities.
    std::erase_if(enemies_, [](const std::unique_ptr<Enemy>& e) { return !e->alive; });
    std::erase_if(pickups_, [](const std::unique_ptr<Pickup>& p) { return !p->alive; });

    // Time limit.
    if (timeRemaining_ >= 0.0f && !player_->dead()) {
        timeRemaining_ -= dt;
        if (timeRemaining_ <= 0.0f) {
            timeRemaining_ = 0.0f;
            loseByTime_ = true;
            loseLevel();
            return;
        }
    }

    // Out of health.
    if (player_->dead()) {
        loseByTime_ = false;
        loseLevel();
        return;
    }
}

void PlayScene::resolvePlayerInteractions() {
    const Rect pb = player_->bounds();

    for (auto& pickup : pickups_) {
        if (!pickup->collected() && pb.intersects(pickup->bounds())) pickup->collect(*this);
    }

    for (auto& enemy : enemies_) {
        if (!enemy->alive || !pb.intersects(enemy->bounds())) continue;
        const bool falling = player_->velocity.y > 60.0f;
        const bool fromAbove = pb.bottom() - enemy->bounds().top() < 32.0f;
        if (falling && fromAbove) {
            defeatEnemy(*enemy);
        } else {
            player_->hurt(enemy->damage(), enemy->center(), *this);
            hurtFlash_ = 1.0f;
        }
    }

    const bool carrying = player_->carried() >= tpGoal_;
    checkout_->setOpen(carrying);
    if (carrying && pb.intersects(checkout_->bounds())) winLevel();
}

void PlayScene::updateCamera(float dt) {
    const float halfView = static_cast<float>(game_.width()) * 0.5f;
    const float maxX = std::max(0.0f, level_.worldWidth - static_cast<float>(game_.width()));
    const float target = clamp(player_->center().x - halfView, 0.0f, maxX);
    cameraX_ = damp(cameraX_, target, 7.0f, dt);
    shakeSeed_ += dt;
}

void PlayScene::shakeCamera(float amount) noexcept {
    shake_ = std::max(shake_, amount);
}

void PlayScene::collectPickup(Pickup& pickup) {
    if (pickup.kind() == PickupKind::ToiletPaper) {
        player_->addCarried(1);
        ++tpCollected_;
        score_ += 100;
        particles_.sparkle(pickup.center(), Color::Cream, 16);
        game_.audio().playSound(game_.assets().sound("pickup_tp"));
        if (tpCollected_ >= tpGoal_) {
            game_.audio().playSound(game_.assets().sound("complete"));
            if (checkout_) particles_.burst(checkout_->center(), Color::Amber, 26, 200.0f, 3.14f, 2.0f, 5.0f, 0.4f, 0.8f, 150.0f);
        }
    } else {
        player_->heal(30.0f);
        score_ += 50;
        particles_.sparkle(pickup.center(), Color::Mint, 16);
        game_.audio().playSound(game_.assets().sound("pickup_health"));
    }
    game_.session().score = score_;
}

void PlayScene::defeatEnemy(Enemy& enemy) {
    enemy.alive = false;
    score_ += enemy.scoreValue();
    game_.session().score = score_;
    particles_.impact(enemy.center(), Color::rgb(255, 214, 92), 18);
    game_.audio().playSound(game_.assets().sound("stomp"));
    shakeCamera(4.0f);
    if (player_) player_->velocity.y = -430.0f;  // bounce
}

void PlayScene::winLevel() {
    if (phase_ != Phase::Playing) return;
    phase_ = Phase::Winning;
    phaseTimer_ = 1.3f;
    const int timeBonus = timeRemaining_ > 0.0f ? static_cast<int>(timeRemaining_ * 5.0f) : 0;
    score_ += 500 + timeBonus;
    game_.session().score = score_;
    if (checkout_) particles_.burst(checkout_->center(), Color::Amber, 48, 280.0f, 3.14f, 2.0f, 5.0f, 0.4f, 0.9f, 220.0f);
    game_.audio().playSound(game_.assets().sound("win"));
}

void PlayScene::loseLevel() {
    if (phase_ != Phase::Playing) return;
    phase_ = Phase::Losing;
    phaseTimer_ = 1.7f;
    game_.audio().playSound(game_.assets().sound("lose"));
    if (player_) particles_.impact(player_->center(), Color::rgb(224, 78, 78), 26);
}

void PlayScene::handleTransitions(float dt) {
    phaseTimer_ -= dt;
    particles_.update(dt);
    updateCamera(dt);
    shake_ = damp(shake_, 0.0f, 6.0f, dt);
    if (phaseTimer_ > 0.0f) return;

    if (phase_ == Phase::Winning) {
        const std::vector<Level> campaign = makeCampaign();
        const int next = levelIndex_ + 1;
        game_.session().score = score_;
        if (next >= static_cast<int>(campaign.size())) {
            game_.session().bestScore = std::max(game_.session().bestScore, score_);
            game_.requestScene(std::make_unique<WinScene>(game_));
        } else {
            game_.requestScene(std::make_unique<PlayScene>(game_, next));
        }
    } else {
        game_.requestScene(std::make_unique<GameOverScene>(game_, loseByTime_));
    }
}

const Input& PlayScene::input() const noexcept { return game_.input(); }
Vec2 PlayScene::playerCenter() const noexcept { return player_ ? player_->center() : Vec2{}; }
Rect PlayScene::playerBounds() const noexcept { return player_ ? player_->bounds() : Rect{}; }
bool PlayScene::playerAlive() const noexcept { return player_ && !player_->dead(); }
Rng& PlayScene::rng() noexcept { return game_.rng(); }
AssetManager& PlayScene::assets() noexcept { return game_.assets(); }
Audio& PlayScene::audio() noexcept { return game_.audio(); }

HudData PlayScene::makeHudData() const {
    HudData data;
    if (player_) {
        data.health = player_->health();
        data.maxHealth = player_->maxHealth();
    }
    data.tpCollected = tpCollected_;
    data.tpGoal = tpGoal_;
    data.score = score_;
    data.level = levelIndex_ + 1;
    data.levelName = level_.name;
    data.timeRemaining = timeRemaining_;
    const bool low = player_ && player_->health() < player_->maxHealth() * 0.3f;
    data.hurtFlash = std::max(hurtFlash_, low ? (0.4f + 0.4f * std::sin(elapsed_ * 6.0f)) : 0.0f);
    return data;
}

void PlayScene::render() {
    renderWorld();
    renderHud();

    if (phase_ == Phase::Paused) {
        renderOverlay();
    } else if (phase_ == Phase::Winning) {
        renderDimBackdrop(Color::rgba(255, 240, 200, 55));
        if (const Font* big = game_.assets().font("display", 32)) {
            const float cx = static_cast<float>(game_.width()) * 0.5f;
            game_.text().drawShadowed(*big, "AISLE CLEARED!", {cx, 210.0f}, Color::Ink,
                                      Color::rgba(255, 255, 255, 190), {3.0f, 4.0f}, TextRenderer::Align::Center);
        }
    } else if (phase_ == Phase::Losing) {
        renderDimBackdrop(Color::rgba(60, 8, 12, 150));
        if (const Font* big = game_.assets().font("display", 32)) {
            const float cx = static_cast<float>(game_.width()) * 0.5f;
            game_.text().drawShadowed(*big, loseByTime_ ? "OUT OF TIME" : "GAME OVER", {cx, 210.0f},
                                      Color::Cream, Color::rgba(0, 0, 0, 190), {3.0f, 4.0f},
                                      TextRenderer::Align::Center);
        }
    }
}

void PlayScene::renderWorld() {
    Renderer& r = game_.renderer();
    const float wf = static_cast<float>(game_.width());
    const float hf = static_cast<float>(game_.height());

    const float shakeX = shake_ > 0.01f ? std::sin(shakeSeed_ * 90.0f) * shake_ : 0.0f;
    const float shakeY = shake_ > 0.01f ? std::cos(shakeSeed_ * 75.0f) * shake_ : 0.0f;

    r.resetCamera();
    r.drawVerticalGradient({0.0f, 0.0f, wf, hf}, level_.skyTop, level_.skyBottom);

    // Parallax shelf silhouettes behind the play field.
    const float spacing = 240.0f;
    const float offset = std::fmod(cameraX_ * 0.4f, spacing);
    for (float bx = -spacing; bx < wf + spacing; bx += spacing) {
        const Rect shelf{bx - offset + 30.0f, 190.0f, 180.0f, level_.floorY - 190.0f};
        r.fillRoundedRect(shelf, 10.0f, Color::rgba(255, 255, 255, 45));
        for (int k = 0; k < 3; ++k) {
            r.fillRect({shelf.x + 14.0f, shelf.y + 40.0f + 62.0f * static_cast<float>(k),
                        shelf.w - 28.0f, 9.0f}, Color::rgba(30, 40, 60, 26));
        }
    }

    r.setCamera({cameraX_ + shakeX, shakeY});

    // Floor.
    r.fillRect({0.0f, level_.floorY, level_.worldWidth, hf - level_.floorY + 220.0f},
               Color::rgb(176, 132, 92));
    r.fillRect({0.0f, level_.floorY, level_.worldWidth, 10.0f}, Color::rgb(214, 178, 128));
    for (float x = 0.0f; x < level_.worldWidth; x += 96.0f) {
        r.fillRect({x, level_.floorY + 14.0f, 4.0f, 60.0f}, Color::rgba(120, 88, 58, 70));
    }

    // Crates.
    for (const Rect& c : level_.crates) {
        if (c.right() < cameraX_ - 80.0f || c.left() > cameraX_ + wf + 80.0f) continue;
        r.fillRoundedRect(c, 8.0f, Color::rgb(198, 150, 96));
        r.drawRoundedRect(c, 8.0f, Color::rgb(148, 106, 64));
        r.fillRect({c.x + 10.0f, c.cy() - 4.0f, c.w - 20.0f, 8.0f}, Color::rgba(148, 106, 64, 130));
    }

    // Entities, back-to-front.
    for (auto& cart : carts_) cart->render(r);
    if (checkout_) checkout_->render(r);
    for (auto& pickup : pickups_) pickup->render(r);
    for (auto& enemy : enemies_) enemy->render(r);
    if (player_) player_->render(r);

    particles_.render(r);
    r.resetCamera();
}

void PlayScene::renderHud() {
    const Font* hudFont = game_.assets().font("display", 12);
    if (!hudFont) return;
    game_.renderer().resetCamera();
    hud_.draw(game_.renderer(), game_.text(), *hudFont, game_.assets(), makeHudData(), game_.width());
}

void PlayScene::renderOverlay() {
    renderDimBackdrop(Color::rgba(10, 12, 20, 185));
    Renderer& r = game_.renderer();
    const float cx = static_cast<float>(game_.width()) * 0.5f;
    if (const Font* titleFont = game_.assets().font("display", 26)) {
        game_.text().drawShadowed(*titleFont, "PAUSED", {cx, 130.0f}, Color::Cream, Color::rgba(0, 0, 0, 180),
                                  {3.0f, 3.0f}, TextRenderer::Align::Center);
    }
    if (const Font* menuFont = game_.assets().font("display", 18)) {
        pauseMenu_.draw(game_.text(), r, *menuFont, cx, 230.0f, 22.0f, Color::rgba(255, 255, 255, 200),
                        Color::Amber, Color::Ink);
    }
}

void PlayScene::renderDimBackdrop(Color tint) {
    Renderer& r = game_.renderer();
    r.resetCamera();
    r.fillRect({0.0f, 0.0f, static_cast<float>(game_.width()), static_cast<float>(game_.height())}, tint);
}

}  // namespace reclaim
