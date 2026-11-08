#include "reclaim/world/Pickup.hpp"

#include <algorithm>
#include <cmath>

#include "reclaim/core/Assets.hpp"
#include "reclaim/scenes/PlayScene.hpp"

namespace reclaim {

Pickup::Pickup(const AssetManager& assets, PickupKind kind, Vec2 position_)
    : kind_(kind), baseY_(position_.y) {
    size = {36.0f, 36.0f};
    position = position_;
    sprite_.texture = assets.texture(kind_ == PickupKind::ToiletPaper ? "tp" : "sanitizer");
}

void Pickup::collect(PlayScene& scene) {
    if (collected_) return;
    collected_ = true;
    collectT_ = 0.0f;
    scene.collectPickup(*this);
}

void Pickup::update(float dt, PlayScene& /*scene*/) {
    bobTime_ += dt;
    spawnT_ += dt;

    if (collected_) {
        collectT_ += dt;
        alive = collectT_ < kPopTime;  // despawn once the pop finishes
    } else {
        position.y = baseY_ + std::sin(bobTime_ * 3.0f) * 4.0f;
    }
}

void Pickup::render(Renderer& renderer) const {
    if (!sprite_.valid()) {
        renderer.fillCircle(center().x, center().y, size.x * 0.5f,
                            kind_ == PickupKind::ToiletPaper ? Color::Cream : Color::Mint);
        return;
    }

    const float appear = ease::outBack(std::clamp(spawnT_ / 0.2f, 0.0f, 1.0f));
    const float pop = collected_ ? std::clamp(collectT_ / kPopTime, 0.0f, 1.0f) : 0.0f;
    const float scale = appear * (1.0f + pop * 0.6f);
    const auto alpha = static_cast<std::uint8_t>(255.0f * (1.0f - pop));

    const float drawW = size.x * scale;
    const float drawH = size.y * scale;
    const Rect dst{center().x - drawW * 0.5f, position.y + (size.y - drawH) * 0.5f, drawW, drawH};

    sprite_.drawTinted(renderer, dst, Color::rgba(255, 255, 255, alpha));
}

}  // namespace reclaim
