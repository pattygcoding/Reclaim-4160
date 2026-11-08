#pragma once

#include <string>

#include "reclaim/core/Font.hpp"
#include "reclaim/core/Math.hpp"
#include "reclaim/core/Renderer.hpp"
#include "reclaim/ui/TextRenderer.hpp"

namespace reclaim {

class AssetManager;

// Everything the in-play HUD needs, snapshotted each frame by the PlayScene.
struct HudData {
    float health = 100.0f;
    float maxHealth = 100.0f;
    int   tpCollected = 0;
    int   tpGoal = 1;
    int   score = 0;
    int   level = 1;
    std::string levelName;
    float timeRemaining = -1.0f;  // seconds; negative means no timer
    float hurtFlash = 0.0f;       // 0..1 pulse when low on health
};

// Draws the heads-up display in screen space (caller should reset the camera).
class Hud {
public:
    void draw(Renderer& renderer, TextRenderer& text, const Font& font,
              AssetManager& assets, const HudData& data, int screenWidth) const;
};

}  // namespace reclaim
