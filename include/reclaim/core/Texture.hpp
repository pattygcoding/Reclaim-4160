#pragma once

#include <SDL2/SDL.h>

#include <cstdint>

#include "reclaim/core/Handles.hpp"

namespace reclaim {

// Move-only owner of an SDL_Texture plus its intrinsic size.
// The AssetManager stores these and hands out `const Texture*` handles; game
// objects reference (never own) them, so lifetimes are simple and leak-free.
class Texture {
public:
    Texture() = default;
    Texture(TexturePtr owned, int width, int height) noexcept;
    ~Texture() = default;

    Texture(Texture&&) noexcept = default;
    Texture& operator=(Texture&&) noexcept = default;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    [[nodiscard]] SDL_Texture* raw() const noexcept { return texture_.get(); }
    [[nodiscard]] int width()  const noexcept { return width_; }
    [[nodiscard]] int height() const noexcept { return height_; }
    [[nodiscard]] float w() const noexcept { return static_cast<float>(width_); }
    [[nodiscard]] float h() const noexcept { return static_cast<float>(height_); }
    [[nodiscard]] bool  valid() const noexcept { return texture_ != nullptr; }

    void setBlendMode(SDL_BlendMode mode) const noexcept;
    void setColorMod(std::uint8_t r, std::uint8_t g, std::uint8_t b) const noexcept;
    void setAlphaMod(std::uint8_t a) const noexcept;
    void resetColorMod() const noexcept;

private:
    TexturePtr texture_;
    int width_ = 0;
    int height_ = 0;
};

}  // namespace reclaim
