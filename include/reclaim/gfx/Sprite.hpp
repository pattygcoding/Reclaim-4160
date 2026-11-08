#pragma once

#include "reclaim/core/Math.hpp"
#include "reclaim/core/Renderer.hpp"
#include "reclaim/core/Texture.hpp"

namespace reclaim {

// A drawable referencing a shared texture. Never owns the texture; the texture
// is kept alive by the AssetManager. Optional per-draw tint and a "stretch"
// factor let gameplay code squash/stretch without touching the texture.
struct Sprite {
    const Texture* texture = nullptr;

    [[nodiscard]] bool valid() const noexcept { return texture != nullptr && texture->valid(); }
    [[nodiscard]] float nativeW() const noexcept { return texture ? texture->w() : 0.0f; }
    [[nodiscard]] float nativeH() const noexcept { return texture ? texture->h() : 0.0f; }

    // Draw preserving aspect ratio into a box aligned to its bottom centre at
    // `bottomCenter`, scaled by `scale` and squashed by (sx, sy).
    void drawAtBottomCenter(Renderer& r, Vec2 bottomCenter, float scaleX, float scaleY,
                            SDL_RendererFlip flip = SDL_FLIP_NONE) const noexcept;

    // Draw into an explicit destination rectangle.
    void drawInto(Renderer& r, Rect dst, SDL_RendererFlip flip = SDL_FLIP_NONE,
                  double angle = 0.0) const noexcept;

    void drawTinted(Renderer& r, Rect dst, Color tint, SDL_RendererFlip flip = SDL_FLIP_NONE) const noexcept;
};

}  // namespace reclaim
