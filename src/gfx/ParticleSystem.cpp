#include "reclaim/gfx/ParticleSystem.hpp"

#include <algorithm>
#include <cmath>

namespace reclaim {

namespace {
constexpr float kPi = 3.14159265f;
constexpr std::size_t kMaxParticles = 800;
}  // namespace

void ParticleSystem::burst(Vec2 origin, Color color, int count, float speed, float spread,
                           float sizeMin, float sizeMax, float lifeMin, float lifeMax,
                           float gravity) {
    for (int i = 0; i < count; ++i) {
        if (particles_.size() >= kMaxParticles) break;
        const float angle = -kPi * 0.5f + rng_.range(-spread, spread);  // biased upward
        const float shot = speed * rng_.range(0.35f, 1.0f);
        Particle p;
        p.position = origin;
        p.velocity = {std::cos(angle) * shot, std::sin(angle) * shot};
        p.maxLife = rng_.range(lifeMin, lifeMax);
        p.life = p.maxLife;
        p.size = rng_.range(sizeMin, sizeMax);
        p.gravity = gravity;
        p.color = color;
        particles_.push_back(p);
    }
}

void ParticleSystem::sparkle(Vec2 origin, Color color, int count) {
    burst(origin, color, count, 160.0f, kPi * 0.9f, 1.5f, 4.0f, 0.3f, 0.7f, 120.0f);
}

void ParticleSystem::dust(Vec2 origin, int count) {
    burst(origin, Color::rgba(220, 214, 200, 210), count, 70.0f, kPi * 0.5f, 2.0f, 4.0f, 0.2f, 0.45f, -30.0f);
}

void ParticleSystem::impact(Vec2 origin, Color color, int count) {
    burst(origin, color, count, 220.0f, kPi, 2.0f, 5.0f, 0.25f, 0.55f, 400.0f);
}

void ParticleSystem::update(float dt) {
    for (auto& p : particles_) {
        p.velocity.y += p.gravity * dt;
        p.position += p.velocity * dt;
        p.life -= dt;
    }
    std::erase_if(particles_, [](const Particle& p) { return p.life <= 0.0f; });
}

void ParticleSystem::render(Renderer& renderer) const {
    for (const auto& p : particles_) {
        const float t = std::clamp(p.life / p.maxLife, 0.0f, 1.0f);
        const float radius = p.size * (0.35f + 0.65f * t);
        const auto alpha = static_cast<std::uint8_t>(255.0f * t);
        renderer.fillCircle(p.position.x, p.position.y, radius, p.color.withAlpha(alpha));
    }
}

}  // namespace reclaim
