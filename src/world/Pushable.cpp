#include "reclaim/world/Pushable.hpp"

#include <algorithm>

#include "reclaim/core/Assets.hpp"
#include "reclaim/scenes/PlayScene.hpp"

namespace reclaim {

Pushable::Pushable(const AssetManager& assets, Vec2 position_) {
    size = {74.0f, 56.0f};
    position = position_;
    sprite_.texture = assets.texture("cart");
}

void Pushable::push(float direction, float dt, PlayScene& /*scene*/) {
    velocity.x = approach(velocity.x, direction * 280.0f, 2600.0f * dt);
    wobble_ += direction * dt * 2.0f;
}

void Pushable::update(float dt, PlayScene& scene) {
    position.y = scene.floorY() - size.y;

    velocity.x = approach(velocity.x, 0.0f, 620.0f * dt);  // rolling friction
    position.x += velocity.x * dt;
    position.x = clamp(position.x, 0.0f, scene.worldWidth() - size.x);

    roll_ += velocity.x * dt * 0.05f;
    wobble_ = damp(wobble_, clamp(velocity.x / 1400.0f, -0.08f, 0.08f), 10.0f, dt);
}

void Pushable::render(Renderer& renderer) const {
    const Rect dst{position.x, position.y, size.x, size.y};
    if (sprite_.valid()) {
        sprite_.drawInto(renderer, dst, SDL_FLIP_NONE, static_cast<double>(wobble_) * 57.2958);
    } else {
        renderer.fillRoundedRect(dst, 10.0f, Color::Teal);
    }
}

}  // namespace reclaim
