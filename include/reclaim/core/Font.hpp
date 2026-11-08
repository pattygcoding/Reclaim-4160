#pragma once

#include <SDL2/SDL_ttf.h>

#include <string_view>

#include "reclaim/core/Handles.hpp"
#include "reclaim/core/Math.hpp"

namespace reclaim {

// Move-only owner of a TTF_Font at a fixed pixel height, with cached measuring.
class Font {
public:
    Font(FontPtr font, int pixelHeight) noexcept;

    Font(Font&&) noexcept = default;
    Font& operator=(Font&&) noexcept = default;
    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;

    [[nodiscard]] TTF_Font* raw() const noexcept { return font_.get(); }
    [[nodiscard]] int pixelHeight() const noexcept { return pixelHeight_; }
    [[nodiscard]] int lineSkip() const noexcept;

    [[nodiscard]] int width(std::string_view text) const noexcept;
    [[nodiscard]] Vec2 measure(std::string_view text) const noexcept;

private:
    FontPtr font_;
    int pixelHeight_;
};

}  // namespace reclaim
