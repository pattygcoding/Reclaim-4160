#pragma once

#include "reclaim/core/Math.hpp"
#include "reclaim/gfx/Sprite.hpp"
#include "reclaim/world/Entity.hpp"

namespace reclaim {

class AssetManager;

enum class PickupKind { ToiletPaper, Sanitizer };

// A collectible that bobs in place, can be picked up by the player, then plays a
// short "pop" animation before disappearing.
class Pickup final : public Entity {
public:
    Pickup(const AssetManager& assets, PickupKind kind, Vec2 position);

    void update(float dt, PlayScene& scene) override;
    void render(Renderer& renderer) const override;

    void collect(PlayScene& scene);
    [[nodiscard]] PickupKind kind() const noexcept { return kind_; }
    [[nodiscard]] bool collected() const noexcept { return collected_; }
    [[nodiscard]] bool finished() const noexcept { return collected_ && collectT_ >= kPopTime; }

private:
    static constexpr float kPopTime = 0.25f;

    PickupKind kind_;
    Sprite sprite_;
    float baseY_;
    float bobTime_ = 0.0f;
    float spawnT_ = 0.0f;
    float collectT_ = 0.0f;
    bool  collected_ = false;
};

}  // namespace reclaim
