#include "reclaim/gfx/Sprite.hpp"

namespace reclaim {

void Sprite::drawAtBottomCenter(Renderer& r, Vec2 bottomCenter, float scaleX, float scaleY,
                                SDL_RendererFlip flip) const noexcept {
    if (!valid()) return;
    const float w = nativeW() * scaleX;
    const float h = nativeH() * scaleY;
    const Rect dst{bottomCenter.x - w * 0.5f, bottomCenter.y - h, w, h};
    r.drawTexture(*texture, dst, flip);
}

void Sprite::drawInto(Renderer& r, Rect dst, SDL_RendererFlip flip, double angle) const noexcept {
    if (valid()) r.drawTexture(*texture, dst, flip, angle);
}

void Sprite::drawTinted(Renderer& r, Rect dst, Color tint, SDL_RendererFlip flip) const noexcept {
    if (valid()) r.drawTextureTinted(*texture, dst, tint, flip);
}

}  // namespace reclaim
