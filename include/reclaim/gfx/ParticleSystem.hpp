#pragma once

#include <vector>

#include "reclaim/core/Math.hpp"
#include "reclaim/core/Random.hpp"
#include "reclaim/core/Renderer.hpp"

namespace reclaim {

// Lightweight pooled particle effect system. Particles live in world space; the
// owning scene sets the camera on the Renderer before rendering.
class ParticleSystem {
public:
    explicit ParticleSystem(Rng& rng) noexcept : rng_(rng) {}

    // Generic burst; most effects are built from this.
    void burst(Vec2 origin, Color color, int count, float speed, float spread,
               float sizeMin, float sizeMax, float lifeMin, float lifeMax, float gravity);

    // Convenience presets.
    void sparkle(Vec2 origin, Color color, int count = 14);
    void dust(Vec2 origin, int count = 6);
    void impact(Vec2 origin, Color color, int count = 10);

    void update(float dt);
    void render(Renderer& renderer) const;

    void clear() noexcept { particles_.clear(); }
    [[nodiscard]] std::size_t count() const noexcept { return particles_.size(); }

private:
    struct Particle {
        Vec2 position;
        Vec2 velocity;
        float life = 0.0f;
        float maxLife = 1.0f;
        float size = 2.0f;
        float gravity = 0.0f;
        Color color{};
    };

    std::vector<Particle> particles_;
    Rng& rng_;
};

}  // namespace reclaim
