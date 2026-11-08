#include "reclaim/scenes/GameOverScene.hpp"

#include <algorithm>
#include <memory>
#include <string>

#include "reclaim/core/Game.hpp"
#include "reclaim/scenes/PlayScene.hpp"
#include "reclaim/scenes/TitleScene.hpp"

namespace reclaim {

GameOverScene::GameOverScene(Game& game, bool timeUp) : Scene(game), timeUp_(timeUp) {
    menu_.add("TRY AGAIN");
    menu_.add("HOME");
}

void GameOverScene::enter() {
    score_ = game_.session().score;
    level_ = game_.session().level;
    game_.audio().playMusic(game_.assets().music("music_lose"));
}

void GameOverScene::update(float dt) {
    time_ += dt;
    menu_.animate(dt);

    const Menu::Result result = menu_.update(game_.input());
    if (result.moved) game_.audio().playSound(game_.assets().sound("select"));
    if (result.confirmed) {
        game_.audio().playSound(game_.assets().sound("confirm"));
        if (result.selected == 0) {
            game_.requestScene(std::make_unique<PlayScene>(game_, std::max(0, level_ - 1)));
        } else {
            game_.requestScene(std::make_unique<TitleScene>(game_));
        }
    }
}

void GameOverScene::render() {
    Renderer& r = game_.renderer();
    TextRenderer& t = game_.text();
    const int w = game_.width();
    const int h = game_.height();
    const float cx = static_cast<float>(w) * 0.5f;

    r.resetCamera();
    r.drawVerticalGradient({0.0f, 0.0f, static_cast<float>(w), static_cast<float>(h)},
                           Color::rgb(70, 26, 34), Color::rgb(120, 40, 52));

    const Font* titleFont = game_.assets().font("display", 36);
    const Font* bodyFont = game_.assets().font("display", 16);
    const Font* menuFont = game_.assets().font("display", 20);

    if (titleFont) {
        t.drawShadowed(*titleFont, "GAME OVER", {cx, 84.0f}, Color::Cream, Color::rgba(0, 0, 0, 160),
                       {3.0f, 4.0f}, TextRenderer::Align::Center);
    }
    if (bodyFont) {
        const std::string reason = timeUp_ ? "Out of time in the aisle!" : "Too many germs got you.";
        t.draw(*bodyFont, reason, {cx, 150.0f}, Color::Amber, TextRenderer::Align::Center);
        t.draw(*bodyFont, "SCORE  " + std::to_string(score_), {cx, 194.0f}, Color::Cream,
               TextRenderer::Align::Center);
        t.draw(*bodyFont, "REACHED AISLE  " + std::to_string(level_), {cx, 226.0f},
               Color::rgba(255, 255, 255, 170), TextRenderer::Align::Center);
    }
    if (menuFont) {
        menu_.draw(t, r, *menuFont, cx, 300.0f, 24.0f, Color::rgba(255, 255, 255, 200), Color::Coral,
                   Color::Ink);
    }
}

}  // namespace reclaim
