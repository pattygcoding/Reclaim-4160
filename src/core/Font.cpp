#include "reclaim/core/Font.hpp"

#include <string>
#include <utility>

namespace reclaim {

Font::Font(FontPtr font, int pixelHeight) noexcept
    : font_(std::move(font)), pixelHeight_(pixelHeight) {}

int Font::lineSkip() const noexcept {
    return font_ ? TTF_FontLineSkip(font_.get()) : pixelHeight_;
}

int Font::width(std::string_view text) const noexcept {
    if (!font_ || text.empty()) return 0;
    int w = 0;
    int h = 0;
    // TTF_SizeUTF8 needs a null-terminated string; string_view may not be.
    TTF_SizeUTF8(font_.get(), std::string(text).c_str(), &w, &h);
    return w;
}

Vec2 Font::measure(std::string_view text) const noexcept {
    if (!font_) return {};
    if (text.empty()) return {0.0f, static_cast<float>(pixelHeight_)};
    int w = 0;
    int h = 0;
    TTF_SizeUTF8(font_.get(), std::string(text).c_str(), &w, &h);
    return {static_cast<float>(w), static_cast<float>(h)};
}

}  // namespace reclaim
