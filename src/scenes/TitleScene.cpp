#include "reclaim/scenes/TitleScene.hpp"

#include <cmath>
#include <memory>

#include "reclaim/core/Game.hpp"
#include "reclaim/scenes/InstructionsScene.hpp"
#include "reclaim/scenes/PlayScene.hpp"

namespace reclaim {

TitleScene::TitleScene(Game& game) : Scene(game) {
    menu_.add("PLAY");
    menu_.add("HOW TO PLAY");
    menu_.add("QUIT");
}

void TitleScene::enter() {
    game_.audio().playMusic(game_.assets().music("music_menu"));
}

void TitleScene::update(float dt) {
    time_ += dt;
    menu_.animate(dt);

    const Menu::Result result = menu_.update(game_.input());
    if (result.moved) game_.audio().playSound(game_.assets().sound("select"));

    if (result.confirmed) {
        game_.audio().playSound(game_.assets().sound("confirm"));
        switch (result.selected) {
            case 0:
                game_.session().level = 1;
                game_.session().score = 0;
                game_.requestScene(std::make_unique<PlayScene>(game_, 0));
                break;
            case 1:
                game_.requestScene(std::make_unique<InstructionsScene>(game_));
                break;
            default:
                game_.quit();
                break;
        }
    }

    if (game_.input().justPressed(SDL_SCANCODE_ESCAPE)) game_.quit();
}

void TitleScene::render() {
    Renderer& r = game_.renderer();
    TextRenderer& t = game_.text();
    const int w = game_.width();
    const int h = game_.height();

    r.resetCamera();
    r.drawVerticalGradient({0.0f, 0.0f, static_cast<float>(w), static_cast<float>(h)},
                           Color::rgb(64, 118, 198), Color::rgb(158, 208, 244));

    // Floating "germs" drifting across the screen.
    for (int i = 0; i < 7; ++i) {
        const float phase = static_cast<float>(i) * 1.7f;
        const float x = std::fmod(static_cast<float>(i) * 168.0f + time_ * 22.0f, static_cast<float>(w) + 90.0f) - 45.0f;
        const float y = 70.0f + std::sin(time_ * 1.2f + phase) * 18.0f + static_cast<float>(i % 3) * 34.0f;
        const float radius = 15.0f + static_cast<float>(i % 3) * 6.0f;
        const Color body = (i % 2 == 0) ? Color::Amber : Color::Mint;
        r.fillCircle(x, y, radius, body);
        r.fillCircle(x - radius * 0.32f, y - radius * 0.2f, radius * 0.18f, Color::Ink);
        r.fillCircle(x + radius * 0.32f, y - radius * 0.2f, radius * 0.18f, Color::Ink);
    }

    const Font* titleFont = game_.assets().font("display", 56);
    const Font* subFont = game_.assets().font("display", 18);
    const Font* menuFont = game_.assets().font("display", 20);
    const Font* hintFont = game_.assets().font("display", 12);
    const float cx = static_cast<float>(w) * 0.5f;

    if (titleFont) {
        t.drawShadowed(*titleFont, "RECLAIM", {cx, 86.0f}, Color::Cream, Color::rgba(18, 28, 56, 190),
                       {4.0f, 6.0f}, TextRenderer::Align::Center);
    }
    if (subFont) {
        t.drawShadowed(*subFont, "4160  -  GROCERY RUN", {cx, 160.0f}, Color::Mint,
                       Color::rgba(18, 28, 56, 160), {2.0f, 3.0f}, TextRenderer::Align::Center);
    }

    const float menuTop = static_cast<float>(h) * 0.52f;
    if (menuFont) {
        menu_.draw(t, r, *menuFont, cx, menuTop, 24.0f, Color::rgba(255, 255, 255, 190), Color::Amber,
                   Color::Ink);
    }

    if (hintFont) {
        t.draw(*hintFont, "W / S  MOVE       SPACE  SELECT", {cx, static_cast<float>(h) - 42.0f},
               Color::rgba(255, 255, 255, 170), TextRenderer::Align::Center);
    }
}

}  // namespace reclaim
