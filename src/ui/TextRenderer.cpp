#include "reclaim/ui/TextRenderer.hpp"

#include <SDL2/SDL_ttf.h>

#include <cstdint>
#include <string>

#include "reclaim/core/Handles.hpp"

namespace reclaim {

namespace {

std::string makeKey(const Font& font, std::string_view text, Color color) {
    std::string key;
    key.reserve(text.size() + 32);
    key += std::to_string(reinterpret_cast<std::uintptr_t>(font.raw()));
    key += '|';
    key += std::to_string(color.r);
    key += ',';
    key += std::to_string(color.g);
    key += ',';
    key += std::to_string(color.b);
    key += ',';
    key += std::to_string(color.a);
    key += '|';
    key.append(text);
    return key;
}

}  // namespace

const Texture* TextRenderer::textureFor(const Font& font, std::string_view text, Color color) {
    std::string key = makeKey(font, text, color);
    if (const auto it = cache_.find(key); it != cache_.end()) return &it->second;

    const SDL_Color sdlColor{color.r, color.g, color.b, color.a};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font.raw(), std::string(text).c_str(), sdlColor);
    if (!surface) return nullptr;

    SDL_Texture* raw = SDL_CreateTextureFromSurface(renderer_.raw(), surface);
    const int w = surface->w;
    const int h = surface->h;
    SDL_FreeSurface(surface);
    if (!raw) return nullptr;
    SDL_SetTextureBlendMode(raw, SDL_BLENDMODE_BLEND);

    const auto result = cache_.emplace(std::move(key), Texture(TexturePtr(raw), w, h));
    return &result.first->second;
}

void TextRenderer::drawTexture(const Texture& tex, Vec2 position, float scale, Align align) {
    const float w = tex.w() * scale;
    const float h = tex.h() * scale;
    float x = position.x;
    if (align == Align::Center) x -= w * 0.5f;
    else if (align == Align::Right) x -= w;
    renderer_.drawTexture(tex, Rect{x, position.y, w, h});
}

void TextRenderer::draw(const Font& font, std::string_view text, Vec2 position, Color color, Align align) {
    if (text.empty() || font.raw() == nullptr) return;
    if (const Texture* tex = textureFor(font, text, color)) drawTexture(*tex, position, 1.0f, align);
}

void TextRenderer::drawScaled(const Font& font, std::string_view text, Vec2 position, Color color,
                              float scale, Align align) {
    if (text.empty() || font.raw() == nullptr) return;
    if (const Texture* tex = textureFor(font, text, color)) drawTexture(*tex, position, scale, align);
}

void TextRenderer::drawShadowed(const Font& font, std::string_view text, Vec2 position, Color color,
                                Color shadow, Vec2 shadowOffset, Align align) {
    draw(font, text, {position.x + shadowOffset.x, position.y + shadowOffset.y}, shadow, align);
    draw(font, text, position, color, align);
}

Vec2 TextRenderer::measure(const Font& font, std::string_view text) const noexcept {
    return font.measure(text);
}

void TextRenderer::pruneIfLarge() {
    if (cache_.size() > 512) cache_.clear();
}

}  // namespace reclaim
