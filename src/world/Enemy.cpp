#include "reclaim/world/Enemy.hpp"

#include <algorithm>
#include <cmath>

#include "reclaim/core/Assets.hpp"
#include "reclaim/scenes/PlayScene.hpp"

namespace reclaim {

Enemy::Enemy(const AssetManager& assets, const EnemySpawn& spawn)
    : kind_(spawn.kind), patrolMin_(spawn.patrolMin), patrolMax_(spawn.patrolMax) {
    size = {46.0f, 46.0f};
    position = spawn.position;
    sprite_.texture = assets.texture(kind_ == EnemyKind::Chaser ? "enemy_chaser" : "enemy_patroller");
    if (!sprite_.valid()) sprite_.texture = assets.texture("enemy");
    facing_ = -1.0f;
}

void Enemy::knockback(Vec2 impulse) noexcept {
    velocity = impulse;
    stun_ = 0.35f;
}

void Enemy::update(float dt, PlayScene& scene) {
    animTime_ += dt;
    bobPhase_ += dt * 6.0f;

    // Recoil after being stomped: slide with friction, ignore AI.
    if (stun_ > 0.0f) {
        stun_ -= dt;
        velocity.x = approach(velocity.x, 0.0f, 1400.0f * dt);
        position.x += velocity.x * dt;
        position.x = clamp(position.x, 0.0f, scene.worldWidth() - size.x);
        position.y = scene.floorY() - size.y;
        return;
    }

    const float speed = kind_ == EnemyKind::Chaser ? 155.0f : 95.0f;
    float dir = facing_;
    bool chasing = false;

    if (kind_ == EnemyKind::Chaser && scene.playerAlive()) {
        const float dx = scene.playerCenter().x - center().x;
        if (std::abs(dx) < 470.0f) {
            dir = sign(dx);
            chasing = true;
        }
    }

    if (!chasing) {
        if (position.x <= patrolMin_) dir = 1.0f;
        else if (position.x >= patrolMax_) dir = -1.0f;
    }

    facing_ = dir;
    position.x += dir * speed * dt;

    if (chasing) {
        position.x = clamp(position.x, 0.0f, scene.worldWidth() - size.x);
    } else {
        position.x = clamp(position.x, patrolMin_, patrolMax_);
    }

    // Germs stay glued to the floor.
    position.y = scene.floorY() - size.y;
    velocity = {};
}

void Enemy::render(Renderer& renderer) const {
    if (!sprite_.valid()) {
        renderer.fillCircle(center().x, center().y, size.x * 0.5f, Color::rgb(220, 180, 60));
        return;
    }

    const float bob = std::sin(bobPhase_) * 3.0f;
    const float squash = 1.0f + std::sin(bobPhase_ * 2.0f) * 0.06f;
    const float sx = (size.x / std::max(1.0f, sprite_.nativeW())) * (2.0f - squash);
    const float sy = (size.y / std::max(1.0f, sprite_.nativeH())) * squash;
    const Vec2 feet{center().x, position.y + size.y + bob};

    sprite_.drawAtBottomCenter(renderer, feet, sx, sy);
}

}  // namespace reclaim
