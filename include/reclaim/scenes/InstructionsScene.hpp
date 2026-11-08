#pragma once

#include "reclaim/scenes/Scene.hpp"

namespace reclaim {

// "How to play" screen. Returns to the title on Space / Enter / Escape.
class InstructionsScene final : public Scene {
public:
    explicit InstructionsScene(Game& game) : Scene(game) {}

    void update(float dt) override;
    void render() override;

private:
    float time_ = 0.0f;
};

}  // namespace reclaim
