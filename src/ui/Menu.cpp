#include "reclaim/ui/Menu.hpp"

namespace reclaim {

void Menu::setSelected(int index) noexcept {
    if (empty()) {
        selected_ = 0;
        return;
    }
    const int n = size();
    selected_ = ((index % n) + n) % n;  // wrap into range
}

Menu::Result Menu::update(const Input& input) noexcept {
    Result result;
    if (empty()) return result;

    bool moved = false;
    if (input.justPressed(SDL_SCANCODE_W) || input.justPressed(SDL_SCANCODE_UP)) {
        setSelected(selected_ - 1);
        moved = true;
    }
    if (input.justPressed(SDL_SCANCODE_S) || input.justPressed(SDL_SCANCODE_DOWN)) {
        setSelected(selected_ + 1);
        moved = true;
    }

    result.selected = selected_;
    result.moved = moved;
    result.confirmed = input.justPressed(SDL_SCANCODE_SPACE) ||
                       input.justPressed(SDL_SCANCODE_RETURN) ||
                       input.justPressed(SDL_SCANCODE_KP_ENTER);
    return result;
}

void Menu::draw(TextRenderer& text, Renderer& renderer, const Font& font, float centerX, float topY,
                float lineGap, Color normal, Color highlight, Color selectedText) {
    const float lineHeight = static_cast<float>(font.pixelHeight());
    float y = topY;

    for (int i = 0; i < size(); ++i) {
        const bool selected = (i == selected_);
        const std::string& label = labels_[static_cast<std::size_t>(i)];
        const Vec2 measured = text.measure(font, label);

        if (selected) {
            const float padX = 28.0f;
            const float padY = 10.0f;
            const Rect pill{centerX - measured.x * 0.5f - padX, y - padY,
                            measured.x + padX * 2.0f, measured.y + padY * 2.0f};
            renderer.fillRoundedRect(pill, pill.h * 0.5f, highlight.withAlpha(220));
            // little "play" triangle to the left of the selected option
            const float cy = pill.cy();
            renderer.fillCircle(pill.x - 10.0f, cy, 5.0f, highlight);
        }

        text.draw(font, label, {centerX, y}, selected ? selectedText : normal, TextRenderer::Align::Center);
        y += lineHeight + lineGap;
    }
}

}  // namespace reclaim
