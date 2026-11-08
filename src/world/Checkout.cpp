#include "reclaim/world/Checkout.hpp"

#include <cmath>

#include "reclaim/core/Assets.hpp"
#include "reclaim/scenes/PlayScene.hpp"

namespace reclaim {

Checkout::Checkout(const AssetManager& assets, Vec2 position_) {
    size = {130.0f, 104.0f};
    position = position_;
    sprite_.texture = assets.texture("checkout");
}

void Checkout::update(float dt, PlayScene& /*scene*/) {
    pulse_ += dt;
}

void Checkout::render(Renderer& renderer) const {
    const Vec2 c = center();

    if (open_) {
        const float glow = 0.5f + 0.5f * std::sin(pulse_ * 4.0f);
        const float radius = size.x * (0.75f + 0.15f * glow);
        renderer.fillCircle(c.x, c.y, radius, Color::rgba(255, 224, 130, static_cast<std::uint8_t>(60.0f * glow)));
    }

    if (sprite_.valid()) {
        sprite_.drawInto(renderer, {position.x, position.y, size.x, size.y});
    } else {
        renderer.fillRoundedRect({position.x, position.y, size.x, size.y}, 12.0f, Color::Teal);
    }
}

}  // namespace reclaim
