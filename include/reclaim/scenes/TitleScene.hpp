#pragma once

#include "reclaim/scenes/Scene.hpp"
#include "reclaim/ui/Menu.hpp"

namespace reclaim {

class TitleScene final : public Scene {
public:
    explicit TitleScene(Game& game);

    void enter() override;
    void update(float dt) override;
    void render() override;

private:
    Menu menu_;
    float time_ = 0.0f;
};

}  // namespace reclaim
