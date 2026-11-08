#pragma once

#include "reclaim/core/Math.hpp"
#include "reclaim/gfx/Sprite.hpp"
#include "reclaim/world/Entity.hpp"

namespace reclaim {

class AssetManager;

// A shopping cart the player can shove along the floor. It rolls (with a little
// friction and a wobble) and is blocked by level bounds and crates.
class Pushable final : public Entity {
public:
    Pushable(const AssetManager& assets, Vec2 position);

    void update(float dt, PlayScene& scene) override;
    void render(Renderer& renderer) const override;

    // Called by the PlayScene when the player overlaps the cart on its side.
    void push(float direction, float dt, PlayScene& scene);

private:
    Sprite sprite_;
    float roll_ = 0.0f;
    float wobble_ = 0.0f;
};

}  // namespace reclaim
