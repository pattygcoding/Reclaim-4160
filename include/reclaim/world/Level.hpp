#pragma once

#include <string>
#include <vector>

#include "reclaim/core/Math.hpp"
#include "reclaim/world/Enemy.hpp"

namespace reclaim {

// A small, data-only description of a level. Scenes build entities from this;
// keeping level layout as plain data (rather than code) keeps gameplay logic out
// of the content and makes new levels trivial to add.
struct Level {
    std::string name = "Aisle 1";
    int index = 1;

    float worldWidth = 2600.0f;
    float floorY = 470.0f;   // y of the top of the floor

    Color skyTop    = Color::rgb(146, 206, 255);
    Color skyBottom = Color::rgb(232, 245, 255);

    std::vector<Rect> crates;          // solid boxes / platforms to jump on
    std::vector<Vec2> tpSpawns;        // toilet-paper pickups (top-left positions)
    std::vector<Vec2> sanitizerSpawns; // health pickups
    std::vector<Vec2> cartSpawns;      // pushable shopping carts
    std::vector<EnemySpawn> enemySpawns;

    Vec2 playerStart{80.0f, 300.0f};
    Vec2 checkoutPos{2400.0f, 300.0f};

    int   tpGoal = 3;
    float timeLimit = -1.0f;   // seconds; negative disables the timer
};

// Built-in level definitions (index 0 == level 1).
[[nodiscard]] std::vector<Level> makeCampaign();

}  // namespace reclaim
