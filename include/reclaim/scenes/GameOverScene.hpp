#pragma once

#include "reclaim/scenes/Scene.hpp"
#include "reclaim/ui/Menu.hpp"

namespace reclaim {

// Shown when the player runs out of health or time.
class GameOverScene final : public Scene {
public:
    GameOverScene(Game& game, bool timeUp = false);

    void enter() override;
    void update(float dt) override;
    void render() override;

private:
    Menu menu_;
    float time_ = 0.0f;
    int score_ = 0;
    int level_ = 1;
    bool timeUp_ = false;
};

}  // namespace reclaim
