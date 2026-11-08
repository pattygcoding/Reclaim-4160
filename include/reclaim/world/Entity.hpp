#pragma once

#include "reclaim/core/Math.hpp"
#include "reclaim/core/Renderer.hpp"

namespace reclaim {

class PlayScene;

// Base class for every object that lives in the world. Entities are updated by
// the PlayScene (which supplies collision queries and shared services through
// its interface) and render themselves in world space.
class Entity {
public:
    virtual ~Entity() = default;

    Vec2 position{};
    Vec2 size{32.0f, 32.0f};
    Vec2 velocity{};
    bool alive = true;

    [[nodiscard]] Rect bounds() const noexcept { return {position.x, position.y, size.x, size.y}; }
    [[nodiscard]] Vec2 center() const noexcept { return bounds().center(); }

    virtual void update(float dt, PlayScene& scene) = 0;
    virtual void render(Renderer& renderer) const = 0;
};

}  // namespace reclaim
