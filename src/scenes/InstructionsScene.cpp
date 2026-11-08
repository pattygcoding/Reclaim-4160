#include "reclaim/scenes/InstructionsScene.hpp"

#include <memory>

#include "reclaim/core/Game.hpp"
#include "reclaim/scenes/TitleScene.hpp"

namespace reclaim {

void InstructionsScene::update(float dt) {
    time_ += dt;
    const Input& input = game_.input();
    if (input.justPressed(SDL_SCANCODE_SPACE) || input.justPressed(SDL_SCANCODE_RETURN) ||
        input.justPressed(SDL_SCANCODE_ESCAPE)) {
        game_.audio().playSound(game_.assets().sound("confirm"));
        game_.requestScene(std::make_unique<TitleScene>(game_));
    }
}

void InstructionsScene::render() {
    Renderer& r = game_.renderer();
    TextRenderer& t = game_.text();
    const int w = game_.width();
    const int h = game_.height();
    const float cx = static_cast<float>(w) * 0.5f;

    r.resetCamera();
    r.drawVerticalGradient({0.0f, 0.0f, static_cast<float>(w), static_cast<float>(h)},
                           Color::rgb(38, 56, 92), Color::rgb(72, 96, 140));

    const Rect panel{static_cast<float>(w) * 0.5f - 340.0f, 44.0f, 680.0f, static_cast<float>(h) - 88.0f};
    r.fillRoundedRect(panel, 22.0f, Color::rgba(16, 18, 28, 180));

    const Font* titleFont = game_.assets().font("display", 22);
    const Font* bodyFont = game_.assets().font("display", 12);
    const Font* hint = game_.assets().font("display", 11);

    if (titleFont) {
        t.draw(*titleFont, "HOW TO PLAY", {cx, panel.y + 26.0f}, Color::Amber, TextRenderer::Align::Center);
    }
    if (bodyFont) {
        static constexpr const char* kLines[] = {
            "GOAL: grab every roll of toilet paper, then",
            "reach the CHECKOUT to clear the aisle.",
            "",
            "MOVE ........... A / D  or  LEFT / RIGHT",
            "JUMP ........... SPACE / W / UP   (hold higher)",
            "STOMP .......... land on a germ to squash it",
            "PAUSE .......... ESC",
            "",
            "Germs drain your health. Sanitizer heals you.",
            "Shove the shopping carts out of your way!",
        };
        float y = panel.y + 84.0f;
        for (const char* line : kLines) {
            t.draw(*bodyFont, line, {cx, y}, Color::Cream, TextRenderer::Align::Center);
            y += 25.0f;
        }
    }
    if (hint) {
        t.draw(*hint, "PRESS SPACE TO RETURN", {cx, panel.bottom() - 34.0f}, Color::Mint,
               TextRenderer::Align::Center);
    }
}

}  // namespace reclaim
