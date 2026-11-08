#include "reclaim/world/Player.hpp"

#include <algorithm>
#include <cmath>

#include "reclaim/core/Assets.hpp"
#include "reclaim/core/Audio.hpp"
#include "reclaim/core/Input.hpp"
#include "reclaim/scenes/PlayScene.hpp"

namespace reclaim {

namespace {
constexpr float kGravity      = 2200.0f;
constexpr float kMaxFall      = 1300.0f;
constexpr float kMoveSpeed    = 300.0f;
constexpr float kGroundAccel  = 2600.0f;
constexpr float kAirAccel     = 1800.0f;
constexpr float kJumpSpeed    = 850.0f;  // ~164 px max jump height at kGravity
constexpr float kJumpCutAccel = 1800.0f;  // softer: even a quick tap clears low crates
constexpr float kCoyoteTime   = 0.10f;
constexpr float kJumpBuffer   = 0.12f;
constexpr float kInvulnTime   = 1.1f;
}  // namespace

Player::Player(const AssetManager& assets) {
    size = {44.0f, 70.0f};
    sprite_.texture = assets.texture("player");
    shadow_.texture = assets.texture("shadow");
    health_ = 100.0f;
    maxHealth_ = 100.0f;
}

void Player::respawn(Vec2 topLeft) noexcept {
    position = topLeft;
    velocity = {};
    grounded_ = false;
}

void Player::heal(float amount) noexcept {
    health_ = std::min(maxHealth_, health_ + amount);
}

void Player::hurt(float amount, Vec2 source, PlayScene& scene) {
    if (invuln_ > 0.0f || dead()) return;

    health_ = std::max(0.0f, health_ - amount);
    invuln_ = kInvulnTime;

    const float dir = (position.x + size.x * 0.5f) >= source.x ? 1.0f : -1.0f;
    velocity.x = dir * 280.0f;
    velocity.y = -340.0f;
    grounded_ = false;
    squashX_ = 1.2f;
    squashY_ = 0.8f;

    scene.particles().impact(center(), Color::rgb(224, 78, 78), 14);
    scene.audio().playSound(scene.assets().sound("hit"));
}

void Player::update(float dt, PlayScene& scene) {
    if (dead()) return;

    invuln_ = std::max(0.0f, invuln_ - dt);
    animTime_ += dt;

    // Recover squash/stretch toward neutral.
    squashX_ = damp(squashX_, 1.0f, 12.0f, dt);
    squashY_ = damp(squashY_, 1.0f, 12.0f, dt);

    applyPhysics(dt, scene);

    // Derive animation state for rendering.
    if (!grounded_) {
        state_ = velocity.y < 0.0f ? State::Jump : State::Fall;
    } else {
        state_ = std::abs(velocity.x) > 24.0f ? State::Run : State::Idle;
    }
}

void Player::applyPhysics(float dt, PlayScene& scene) {
    const Input& input = scene.input();

    const bool left = input.isDown(SDL_SCANCODE_A) || input.isDown(SDL_SCANCODE_LEFT);
    const bool right = input.isDown(SDL_SCANCODE_D) || input.isDown(SDL_SCANCODE_RIGHT);
    const float dir = (right ? 1.0f : 0.0f) - (left ? 1.0f : 0.0f);
    if (dir != 0.0f) facing_ = dir;

    const float targetVx = dir * kMoveSpeed;
    const float accel = grounded_ ? kGroundAccel : kAirAccel;
    velocity.x = approach(velocity.x, targetVx, accel * dt);

    const bool jumpPressed = input.justPressed(SDL_SCANCODE_SPACE) ||
                             input.justPressed(SDL_SCANCODE_W) ||
                             input.justPressed(SDL_SCANCODE_UP);
    jumpHeld_ = input.isDown(SDL_SCANCODE_SPACE) || input.isDown(SDL_SCANCODE_W) ||
                input.isDown(SDL_SCANCODE_UP);

    if (jumpPressed) jumpBuffer_ = kJumpBuffer;
    jumpBuffer_ = std::max(0.0f, jumpBuffer_ - dt);
    coyote_ = grounded_ ? kCoyoteTime : std::max(0.0f, coyote_ - dt);

    if (jumpBuffer_ > 0.0f && coyote_ > 0.0f) {
        velocity.y = -kJumpSpeed;
        jumpBuffer_ = 0.0f;
        coyote_ = 0.0f;
        grounded_ = false;
        squashX_ = 0.8f;
        squashY_ = 1.25f;
        scene.particles().dust({position.x + size.x * 0.5f, position.y + size.y});
        scene.audio().playSound(scene.assets().sound("jump"));
    }

    if (!jumpHeld_ && velocity.y < 0.0f) {
        velocity.y = approach(velocity.y, 0.0f, kJumpCutAccel * dt);
    }

    velocity.y = std::min(velocity.y + kGravity * dt, kMaxFall);

    position.x += velocity.x * dt;
    resolveCollisions(scene);
    position.y += velocity.y * dt;
    resolveCollisions(scene);
}

void Player::resolveCollisions(PlayScene& scene) {
    const bool wasGrounded = grounded_;
    grounded_ = false;

    for (const Rect& solid : scene.solidRects()) {
        if (!bounds().intersects(solid)) continue;

        const Rect b = bounds();
        const float overlapLeft   = b.right() - solid.left();
        const float overlapRight  = solid.right() - b.left();
        const float overlapTop    = b.bottom() - solid.top();
        const float overlapBottom = solid.bottom() - b.top();

        const float minX = std::min(overlapLeft, overlapRight);
        const float minY = std::min(overlapTop, overlapBottom);

        if (minX < minY) {
            if (overlapLeft < overlapRight) position.x = solid.left() - size.x;
            else position.x = solid.right();
            // Keep horizontal velocity on side hits (position is re-clamped each
            // frame): this lets the player carry momentum up and over a crate
            // they are pressed against, instead of sticking to its face.
        } else {
            if (overlapTop < overlapBottom) {
                position.y = solid.top() - size.y;
                if (velocity.y > 0.0f) {
                    velocity.y = 0.0f;
                    grounded_ = true;
                }
            } else {
                position.y = solid.bottom();
                if (velocity.y < 0.0f) velocity.y = 0.0f;
            }
        }
    }

    if (grounded_ && !wasGrounded) {
        scene.particles().dust({position.x + size.x * 0.5f, position.y + size.y}, 5);
    }

    position.x = clamp(position.x, 0.0f, scene.worldWidth() - size.x);
}

void Player::render(Renderer& renderer) const {
    const float feetX = position.x + size.x * 0.5f;
    const float feetY = position.y + size.y;

    if (shadow_.valid()) {
        const float shW = size.x * 1.15f;
        const float shH = shW * (shadow_.nativeH() / shadow_.nativeW());
        shadow_.drawInto(renderer, {feetX - shW * 0.5f, feetY - shH * 0.55f, shW, shH});
    }

    if (invuln_ > 0.0f && (static_cast<int>(invuln_ * 20.0f) % 2 == 0)) return;

    const float sx = (size.x / std::max(1.0f, sprite_.nativeW())) * squashX_;
    const float sy = (size.y / std::max(1.0f, sprite_.nativeH())) * squashY_;
    const SDL_RendererFlip flip = facing_ < 0.0f ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    sprite_.drawAtBottomCenter(renderer, {feetX, feetY}, sx, sy, flip);
}

}  // namespace reclaim
