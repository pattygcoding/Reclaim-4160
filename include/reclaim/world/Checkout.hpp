#pragma once

#include "reclaim/core/Math.hpp"
#include "reclaim/gfx/Sprite.hpp"
#include "reclaim/world/Entity.hpp"

namespace reclaim {

class AssetManager;

// The level's goal. Reaching it while carrying the required toilet paper wins
// the level. It glows once it becomes the objective (all TP collected).
class Checkout final : public Entity {
public:
    Checkout(const AssetManager& assets, Vec2 position);

    void update(float dt, PlayScene& scene) override;
    void render(Renderer& renderer) const override;

    void setOpen(bool open) noexcept { open_ = open; }
    [[nodiscard]] bool open() const noexcept { return open_; }

private:
    Sprite sprite_;
    Sprite glow_;
    bool  open_ = false;
    float pulse_ = 0.0f;
};

}  // namespace reclaim
