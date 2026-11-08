#pragma once

#include "reclaim/core/Math.hpp"
#include "reclaim/gfx/Sprite.hpp"
#include "reclaim/world/Entity.hpp"

namespace reclaim {

class AssetManager;

enum class EnemyKind { Patroller, Chaser };

struct EnemySpawn {
    EnemyKind kind = EnemyKind::Patroller;
    Vec2 position{};
    float patrolMin = 0.0f;
    float patrolMax = 0.0f;
};

// The "germs" that roam the aisles. Patrollers pace a fixed range; Chasers
// home in on the player when within range, then return home when the player
// escapes. Both deal contact damage.
class Enemy final : public Entity {
public:
    Enemy(const AssetManager& assets, const EnemySpawn& spawn);

    void update(float dt, PlayScene& scene) override;
    void render(Renderer& renderer) const override;

    [[nodiscard]] float damage() const noexcept { return kind_ == EnemyKind::Chaser ? 14.0f : 10.0f; }
    [[nodiscard]] int   scoreValue() const noexcept { return kind_ == EnemyKind::Chaser ? 250 : 150; }
    [[nodiscard]] EnemyKind kind() const noexcept { return kind_; }

    void knockback(Vec2 impulse) noexcept;

private:
    EnemyKind kind_;
    Sprite sprite_;
    float patrolMin_;
    float patrolMax_;
    float facing_ = -1.0f;
    float animTime_ = 0.0f;
    float stun_ = 0.0f;
    float bobPhase_ = 0.0f;
};

}  // namespace reclaim
