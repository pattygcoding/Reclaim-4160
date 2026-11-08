#include "reclaim/core/Texture.hpp"

#include <utility>

namespace reclaim {

Texture::Texture(TexturePtr owned, int width, int height) noexcept
    : texture_(std::move(owned)), width_(width), height_(height) {}

void Texture::setBlendMode(SDL_BlendMode mode) const noexcept {
    if (texture_) SDL_SetTextureBlendMode(texture_.get(), mode);
}

void Texture::setColorMod(std::uint8_t r, std::uint8_t g, std::uint8_t b) const noexcept {
    if (texture_) SDL_SetTextureColorMod(texture_.get(), r, g, b);
}

void Texture::setAlphaMod(std::uint8_t a) const noexcept {
    if (texture_) SDL_SetTextureAlphaMod(texture_.get(), a);
}

void Texture::resetColorMod() const noexcept {
    setColorMod(255, 255, 255);
    setAlphaMod(255);
}

}  // namespace reclaim
