#include "reclaim/core/Renderer.hpp"

#include <algorithm>
#include <cmath>
#include <utility>

namespace reclaim {

Renderer::Renderer(RendererPtr renderer, int logicalWidth, int logicalHeight) noexcept
    : renderer_(std::move(renderer)), width_(logicalWidth), height_(logicalHeight) {}

SDL_FRect Renderer::toScreen(const Rect& r) const noexcept {
    return SDL_FRect{r.x - camera_.x, r.y - camera_.y, r.w, r.h};
}

SDL_Rect Renderer::toScreenI(const Rect& r) const noexcept {
    return SDL_Rect{static_cast<int>(std::lround(r.x - camera_.x)),
                    static_cast<int>(std::lround(r.y - camera_.y)),
                    static_cast<int>(std::lround(r.w)),
                    static_cast<int>(std::lround(r.h))};
}

void Renderer::beginFrame(Color clear) noexcept {
    camera_ = {};
    SDL_SetRenderDrawBlendMode(renderer_.get(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer_.get(), clear.r, clear.g, clear.b, clear.a);
    SDL_RenderClear(renderer_.get());
    current_ = clear;
}

void Renderer::endFrame() noexcept { SDL_RenderPresent(renderer_.get()); }

void Renderer::setDrawColor(Color c) noexcept {
    current_ = c;
    SDL_SetRenderDrawColor(renderer_.get(), c.r, c.g, c.b, c.a);
}

void Renderer::setBlendMode(SDL_BlendMode mode) noexcept {
    SDL_SetRenderDrawBlendMode(renderer_.get(), mode);
}

void Renderer::drawTexture(const Texture& tex, Rect dst, SDL_RendererFlip flip, double angle) const noexcept {
    if (!tex.valid()) return;
    SDL_FRect d = toScreen(dst);
    if (angle == 0.0 && flip == SDL_FLIP_NONE) {
        SDL_RenderCopyF(renderer_.get(), tex.raw(), nullptr, &d);
    } else {
        SDL_RenderCopyExF(renderer_.get(), tex.raw(), nullptr, &d, angle, nullptr, flip);
    }
}

void Renderer::drawTexture(const Texture& tex, Rect src, Rect dst, SDL_RendererFlip flip, double angle) const noexcept {
    if (!tex.valid()) return;
    const SDL_Rect s{static_cast<int>(std::lround(src.x)), static_cast<int>(std::lround(src.y)),
                     static_cast<int>(std::lround(src.w)), static_cast<int>(std::lround(src.h))};
    SDL_FRect d = toScreen(dst);
    SDL_RenderCopyExF(renderer_.get(), tex.raw(), &s, &d, angle, nullptr, flip);
}

void Renderer::drawTextureTinted(const Texture& tex, Rect dst, Color tint, SDL_RendererFlip flip, double angle) const noexcept {
    if (!tex.valid()) return;
    tex.setColorMod(tint.r, tint.g, tint.b);
    tex.setAlphaMod(tint.a);
    drawTexture(tex, dst, flip, angle);
    tex.resetColorMod();
}

void Renderer::fillRect(Rect r, Color c) const noexcept {
    SDL_FRect d = toScreen(r);
    SDL_SetRenderDrawColor(renderer_.get(), c.r, c.g, c.b, c.a);
    SDL_RenderFillRectF(renderer_.get(), &d);
}

void Renderer::drawRect(Rect r, Color c) const noexcept {
    SDL_FRect d = toScreen(r);
    SDL_SetRenderDrawColor(renderer_.get(), c.r, c.g, c.b, c.a);
    SDL_RenderDrawRectF(renderer_.get(), &d);
}

void Renderer::fillCircle(float cx, float cy, float radius, Color c) const noexcept {
    if (radius <= 0.0f) return;
    SDL_SetRenderDrawColor(renderer_.get(), c.r, c.g, c.b, c.a);
    const float sx = cx - camera_.x;
    const float sy = cy - camera_.y;
    const int ir = static_cast<int>(std::ceil(radius));
    for (int dy = -ir; dy <= ir; ++dy) {
        const float dx = std::sqrt(std::max(0.0f, radius * radius - static_cast<float>(dy * dy)));
        SDL_RenderDrawLineF(renderer_.get(), sx - dx, sy + static_cast<float>(dy),
                            sx + dx, sy + static_cast<float>(dy));
    }
}

void Renderer::drawVerticalGradient(Rect area, Color top, Color bottom, int steps) const noexcept {
    if (steps < 1) steps = 1;
    const float stepH = area.h / static_cast<float>(steps);
    for (int i = 0; i < steps; ++i) {
        const float t = static_cast<float>(i) / static_cast<float>(steps - 1 == 0 ? 1 : steps - 1);
        const auto mix = [](std::uint8_t a, std::uint8_t b, float f) {
            return static_cast<std::uint8_t>(static_cast<float>(a) + (static_cast<float>(b) - static_cast<float>(a)) * f);
        };
        const Color c{mix(top.r, bottom.r, t), mix(top.g, bottom.g, t), mix(top.b, bottom.b, t), 255};
        fillRect({area.x, area.y + stepH * static_cast<float>(i), area.w, stepH + 1.0f}, c);
    }
}

void Renderer::fillRoundedRect(Rect r, float radius, Color c) const noexcept {
    radius = std::min(radius, std::min(r.w, r.h) * 0.5f);
    if (radius <= 0.5f) { fillRect(r, c); return; }
    fillRect({r.x + radius, r.y, r.w - 2.0f * radius, r.h}, c);
    fillRect({r.x, r.y + radius, r.w, r.h - 2.0f * radius}, c);
    fillCircle(r.x + radius, r.y + radius, radius, c);
    fillCircle(r.right() - radius, r.y + radius, radius, c);
    fillCircle(r.x + radius, r.bottom() - radius, radius, c);
    fillCircle(r.right() - radius, r.bottom() - radius, radius, c);
}

void Renderer::drawRoundedRect(Rect r, float radius, Color c) const noexcept {
    // Simple outline approximation (used rarely for focus rings).
    (void)radius;
    drawRect(r, c);
}

void Renderer::setClip(const std::optional<Rect>& clip) const noexcept {
    if (clip) {
        const SDL_Rect rc = toScreenI(*clip);
        SDL_RenderSetClipRect(renderer_.get(), &rc);
    } else {
        SDL_RenderSetClipRect(renderer_.get(), nullptr);
    }
}

}  // namespace reclaim
