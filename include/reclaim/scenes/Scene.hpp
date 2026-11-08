#pragma once

namespace reclaim {

class Game;

// A screen / game state (title, play, game-over, ...). The Game owns exactly one
// active scene and swaps between them on request, so each scene is self-contained
// and independent of the others.
class Scene {
public:
    explicit Scene(Game& game) noexcept : game_(game) {}
    virtual ~Scene() = default;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    virtual void enter() {}
    virtual void exit() {}
    virtual void update(float dt) = 0;
    virtual void render() = 0;

protected:
    Game& game_;
};

}  // namespace reclaim
