#include "reclaim/scenes/WinScene.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>

#include "reclaim/core/Game.hpp"
#include "reclaim/scenes/PlayScene.hpp"
#include "reclaim/scenes/TitleScene.hpp"

namespace reclaim {

WinScene::WinScene(Game& game) : Scene(game) {
    menu_.add("PLAY AGAIN");
    menu_.add("HOME");
}

void WinScene::enter() {
    score_ = game_.session().score;
    best_ = std::max(score_, game_.session().bestScore);
    game_.session().bestScore = best_;
    game_.audio().playMusic(game_.assets().music("music_win"));
}

void WinScene::update(float dt) {
    time_ += dt;
    menu_.animate(dt);

    const Menu::Result result = menu_.update(game_.input());
    if (result.moved) game_.audio().playSound(game_.assets().sound("select"));
    if (result.confirmed) {
        game_.audio().playSound(game_.assets().sound("confirm"));
        if (result.selected == 0) {
            game_.session().level = 1;
            game_.session().score = 0;
            game_.requestScene(std::make_unique<PlayScene>(game_, 0));
        } else {
            game_.requestScene(std::make_unique<TitleScene>(game_));
        }
    }
}

void WinScene::render() {
    Renderer& r = game_.renderer();
    TextRenderer& t = game_.text();
    const int w = game_.width();
    const int h = game_.height();
    const float cx = static_cast<float>(w) * 0.5f;

    r.resetCamera();
    r.drawVerticalGradient({0.0f, 0.0f, static_cast<float>(w), static_cast<float>(h)},
                           Color::rgb(255, 224, 150), Color::rgb(255, 246, 220));

    // Simple confetti drift.
    for (int i = 0; i < 24; ++i) {
        const float x = std::fmod(static_cast<float>(i) * 97.0f + time_ * 40.0f, static_cast<float>(w) + 30.0f);
        const float y = std::fmod(static_cast<float>(i) * 53.0f + time_ * 80.0f, static_cast<float>(h));
        const Color c = (i % 3 == 0) ? Color::Teal : (i % 3 == 1 ? Color::Coral : Color::Purple);
        r.fillRect({x, y, 8.0f, 8.0f}, c.withAlpha(200));
    }

    const Font* titleFont = game_.assets().font("display", 40);
    const Font* bodyFont = game_.assets().font("display", 18);
    const Font* menuFont = game_.assets().font("display", 20);

    if (titleFont) {
        t.drawShadowed(*titleFont, "YOU WIN!", {cx, 70.0f}, Color::Ink, Color::rgba(255, 255, 255, 180),
                       {3.0f, 4.0f}, TextRenderer::Align::Center);
    }
    if (bodyFont) {
        t.draw(*bodyFont, "The aisle is reclaimed!", {cx, 130.0f}, Color::Teal, TextRenderer::Align::Center);
        t.draw(*bodyFont, "FINAL SCORE  " + std::to_string(score_), {cx, 172.0f}, Color::Ink,
               TextRenderer::Align::Center);
        t.draw(*bodyFont, "BEST  " + std::to_string(best_), {cx, 206.0f}, Color::Purple,
               TextRenderer::Align::Center);
    }
    if (menuFont) {
        menu_.draw(t, r, *menuFont, cx, 290.0f, 24.0f, Color::rgba(34, 30, 48, 200), Color::Amber, Color::Ink);
    }
}

}  // namespace reclaim
