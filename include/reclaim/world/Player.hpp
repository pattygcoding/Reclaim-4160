#pragma once

#include "reclaim/core/Math.hpp"
#include "reclaim/gfx/Sprite.hpp"
#include "reclaim/world/Entity.hpp"

namespace reclaim {

class AssetManager;

// The shopper. Handles input-driven movement, gravity, AABB collision against
// the level's solids, coyote-time/jump-buffered jumping, health with
// invulnerability frames, and a bit of squash-and-stretch for game feel.
class Player final : public Entity {
public:
    explicit Player(const AssetManager& assets);

    void respawn(Vec2 topLeft) noexcept;
    void update(float dt, PlayScene& scene) override;
    void render(Renderer& renderer) const override;

    void hurt(float amount, Vec2 source, PlayScene& scene);
    void heal(float amount) noexcept;
    void addCarried(int amount) noexcept { carried_ += amount; }

    [[nodiscard]] float health() const noexcept { return health_; }
    [[nodiscard]] float maxHealth() const noexcept { return maxHealth_; }
    [[nodiscard]] bool  dead() const noexcept { return health_ <= 0.0f; }
    [[nodiscard]] bool  grounded() const noexcept { return grounded_; }
    [[nodiscard]] bool  invulnerable() const noexcept { return invuln_ > 0.0f; }
    [[nodiscard]] int   carried() const noexcept { return carried_; }
    [[nodiscard]] bool  facingRight() const noexcept { return facing_ >= 0.0f; }

private:
    enum class State { Idle, Run, Jump, Fall };

    void applyPhysics(float dt, PlayScene& scene);
    void resolveCollisions(PlayScene& scene);

    Sprite sprite_;
    Sprite shadow_;

    State state_ = State::Idle;
    float facing_ = 1.0f;
    float health_ = 100.0f;
    float maxHealth_ = 100.0f;
    float invuln_ = 0.0f;
    float coyote_ = 0.0f;
    float jumpBuffer_ = 0.0f;
    float animTime_ = 0.0f;
    float squashX_ = 1.0f;
    float squashY_ = 1.0f;
    int   carried_ = 0;
    bool  grounded_ = false;
    bool  jumpHeld_ = false;
};

}  // namespace reclaim
