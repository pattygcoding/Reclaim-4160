#pragma once

#include <SDL2/SDL.h>

#include <optional>

#include "reclaim/core/Handles.hpp"
#include "reclaim/core/Math.hpp"
#include "reclaim/core/Texture.hpp"

namespace reclaim {

// High-level drawing facade over SDL_Renderer.
//
// A single "camera offset" is applied to every draw call so scenes can render
// the world in world-space and the UI in screen-space simply by resetting the
// offset. Shape helpers (rounded rects, circles) are implemented from primitive
// draws so the project does not depend on SDL2_gfx.
class Renderer {
public:
    Renderer(RendererPtr renderer, int logicalWidth, int logicalHeight) noexcept;

    [[nodiscard]] SDL_Renderer* raw() const noexcept { return renderer_.get(); }
    [[nodiscard]] int width()  const noexcept { return width_; }
    [[nodiscard]] int height() const noexcept { return height_; }

    void setCamera(Vec2 offset) noexcept { camera_ = offset; }
    void resetCamera() noexcept { camera_ = {}; }

    void beginFrame(Color clear) noexcept;
    void endFrame() noexcept;

    void setDrawColor(Color c) noexcept;
    void setBlendMode(SDL_BlendMode mode) noexcept;

    // Textures
    void drawTexture(const Texture& tex, Rect dst, SDL_RendererFlip flip = SDL_FLIP_NONE,
                     double angle = 0.0) const noexcept;
    void drawTexture(const Texture& tex, Rect src, Rect dst, SDL_RendererFlip flip = SDL_FLIP_NONE,
                     double angle = 0.0) const noexcept;
    // Draw a texture tinted + alpha-modulated (restores modifiers afterwards).
    void drawTextureTinted(const Texture& tex, Rect dst, Color tint,
                           SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0) const noexcept;

    // Primitive shapes
    void fillRect(Rect r, Color c) const noexcept;
    void drawRect(Rect r, Color c) const noexcept;
    void fillRoundedRect(Rect r, float radius, Color c) const noexcept;
    void drawRoundedRect(Rect r, float radius, Color c) const noexcept;
    void fillCircle(float cx, float cy, float radius, Color c) const noexcept;
    void drawVerticalGradient(Rect area, Color top, Color bottom, int steps = 32) const noexcept;

    // Clipping (screen-space rectangle). std::nullopt disables clipping.
    void setClip(const std::optional<Rect>& clip) const noexcept;

private:
    [[nodiscard]] SDL_FRect toScreen(const Rect& r) const noexcept;
    [[nodiscard]] SDL_Rect  toScreenI(const Rect& r) const noexcept;

    RendererPtr renderer_;
    int width_;
    int height_;
    Vec2 camera_{};
    mutable Color current_{};
};

}  // namespace reclaim
