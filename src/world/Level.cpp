#include "reclaim/world/Level.hpp"

namespace reclaim {

namespace {

constexpr float kFloorY  = 470.0f;
constexpr float kEnemyY  = kFloorY - 46.0f;   // enemies are 46 tall
constexpr float kPickupY = kFloorY - 84.0f;   // ground-level pickup
constexpr float kCartY   = kFloorY - 56.0f;   // cart is 56 tall

constexpr float kCrateW = 110.0f;
// The player's jump clears ~164 px over a ~145 px horizontal window. Crates are
// kept <=100 px tall and 110 px wide so every obstacle is reliably hoppable and
// each level is always solvable.
// Crate heights kept <=75 px so that even a quick tap-jump (~95 px) clears them.
constexpr float kLow  = 65.0f;   // crate height (top = floor - height)
constexpr float kMid  = 75.0f;
constexpr float kHigh = 70.0f;
constexpr float kVLow = 60.0f;

Rect crate(float x, float height) { return {x, kFloorY - height, kCrateW, height}; }

EnemySpawn patroller(Vec2 pos, float lo, float hi) { return {EnemyKind::Patroller, pos, lo, hi}; }
EnemySpawn chaser(Vec2 pos, float lo, float hi) { return {EnemyKind::Chaser, pos, lo, hi}; }

// A pickup resting just above a crate.
Vec2 onCrate(float x, const Rect& c) { return {x, c.y - 40.0f}; }

}  // namespace

std::vector<Level> makeCampaign() {
    std::vector<Level> levels;

    // ---------------- Level 1 : Paper Run ----------------
    {
        Level lvl;
        lvl.name = "Aisle 1 - Paper Run";
        lvl.index = 1;
        lvl.worldWidth = 2600.0f;
        lvl.floorY = kFloorY;
        lvl.skyTop = Color::rgb(132, 200, 248);
        lvl.skyBottom = Color::rgb(226, 244, 255);

        const Rect c1 = crate(700.0f, kLow);
        const Rect c2 = crate(980.0f, kMid);
        const Rect c3 = crate(1500.0f, kLow);
        const Rect c4 = crate(1900.0f, kHigh);
        lvl.crates = {c1, c2, c3, c4};

        lvl.tpSpawns = {
            {520.0f, kPickupY},
            onCrate(1010.0f, c2),
            {2250.0f, kPickupY},
        };
        lvl.sanitizerSpawns = {onCrate(1930.0f, c4)};
        lvl.cartSpawns = {{1250.0f, kCartY}};
        lvl.enemySpawns = {
            patroller({470.0f, kEnemyY}, 320.0f, 690.0f),
            chaser({1300.0f, kEnemyY}, 1150.0f, 1480.0f),
            patroller({2150.0f, kEnemyY}, 2080.0f, 2400.0f),
        };
        lvl.playerStart = {90.0f, kFloorY - 70.0f};
        lvl.checkoutPos = {2450.0f, kFloorY - 104.0f};
        lvl.tpGoal = 3;
        lvl.timeLimit = -1.0f;
        levels.push_back(std::move(lvl));
    }

    // ---------------- Level 2 : Germ Gauntlet ----------------
    {
        Level lvl;
        lvl.name = "Aisle 2 - Germ Gauntlet";
        lvl.index = 2;
        lvl.worldWidth = 3300.0f;
        lvl.floorY = kFloorY;
        lvl.skyTop = Color::rgb(255, 196, 168);
        lvl.skyBottom = Color::rgb(255, 236, 214);

        const Rect a = crate(520.0f, kVLow);
        const Rect b = crate(980.0f, kMid);
        const Rect c = crate(1500.0f, kLow);
        const Rect d = crate(1950.0f, kHigh);
        const Rect e = crate(2500.0f, kVLow);
        const Rect f = crate(2900.0f, kMid);
        lvl.crates = {a, b, c, d, e, f};

        lvl.tpSpawns = {
            {430.0f, kPickupY},
            onCrate(1010.0f, b),
            onCrate(2000.0f, d),
            onCrate(2930.0f, f),
        };
        lvl.sanitizerSpawns = {
            {1250.0f, kPickupY},
            onCrate(2530.0f, e),
        };
        lvl.cartSpawns = {{760.0f, kCartY}, {2250.0f, kCartY}};
        lvl.enemySpawns = {
            patroller({760.0f, kEnemyY}, 660.0f, 960.0f),
            chaser({1300.0f, kEnemyY}, 1150.0f, 1480.0f),
            patroller({2300.0f, kEnemyY}, 2150.0f, 2450.0f),
            chaser({2750.0f, kEnemyY}, 2620.0f, 2880.0f),
        };
        lvl.playerStart = {70.0f, kFloorY - 70.0f};
        lvl.checkoutPos = {3160.0f, kFloorY - 104.0f};
        lvl.tpGoal = 4;
        lvl.timeLimit = 120.0f;
        levels.push_back(std::move(lvl));
    }

    return levels;
}

}  // namespace reclaim
