#pragma once

#include "reclaim/scenes/Scene.hpp"
#include "reclaim/ui/Menu.hpp"

namespace reclaim {

// Shown after clearing the final level of the campaign.
class WinScene final : public Scene {
public:
    explicit WinScene(Game& game);

    void enter() override;
    void update(float dt) override;
    void render() override;

private:
    Menu menu_;
    float time_ = 0.0f;
    int score_ = 0;
    int best_ = 0;
};

}  // namespace reclaim
