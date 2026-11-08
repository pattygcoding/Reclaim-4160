#include "reclaim/world/Level.hpp"

#include <algorithm>
#include <vector>

namespace reclaim {

namespace {

constexpr float kFloorY  = 470.0f;
constexpr float kEnemyY  = kFloorY - 46.0f;   // enemies are 46 tall
constexpr float kPickupY = kFloorY - 84.0f;   // ground-level pickup
constexpr float kCartY   = kFloorY - 56.0f;   // cart is 56 tall
constexpr float kCrateW  = 110.0f;

constexpr int kLevelCount = 10;

struct Sky { Color top; Color bottom; };
const Sky kSkies[] = {
    {Color::rgb(132, 200, 248), Color::rgb(226, 244, 255)},
    {Color::rgb(255, 196, 168), Color::rgb(255, 236, 214)},
    {Color::rgb(176, 224, 196), Color::rgb(232, 248, 236)},
    {Color::rgb(206, 196, 248), Color::rgb(238, 234, 255)},
    {Color::rgb(255, 214, 150), Color::rgb(255, 240, 206)},
};
constexpr int kSkyCount = static_cast<int>(sizeof(kSkies) / sizeof(kSkies[0]));

const char* const kNames[kLevelCount] = {
    "Aisle 1 - Paper Run",
    "Aisle 2 - Germ Gauntlet",
    "Aisle 3 - Canned Chaos",
    "Aisle 4 - Dairy Dash",
    "Aisle 5 - Freezer Burn",
    "Aisle 6 - Produce Panic",
    "Aisle 7 - Checkout Crush",
    "Aisle 8 - Restock Riot",
    "Aisle 9 - Bulk Brawl",
    "Aisle 10 - The Last Roll",
};

// Crate heights kept <=75 px so even a quick tap-jump (~95 px) clears them.
constexpr float kCrateHeights[] = {65.0f, 75.0f, 60.0f, 70.0f};

// Builds one solvable aisle. Difficulty ramps with the index: longer aisles,
// more enemies, crates, pickups and carts.
Level makeLevel(int i) {
    Level lvl;
    lvl.index = i + 1;
    lvl.name = kNames[i];
    lvl.worldWidth = 2400.0f + static_cast<float>(i) * 240.0f;
    lvl.floorY = kFloorY;
    lvl.timeLimit = -1.0f;  // no timer: keep every aisle winnable without rushing

    const Sky& sky = kSkies[i % kSkyCount];
    lvl.skyTop = sky.top;
    lvl.skyBottom = sky.bottom;

    lvl.playerStart = {90.0f, kFloorY - 70.0f};
    lvl.checkoutPos = {lvl.worldWidth - 150.0f, kFloorY - 104.0f};

    const int crateCount = 3 + i / 2;
    const int tpGoal = std::min(3 + i / 2, 6);
    lvl.tpGoal = tpGoal;

    // Crates spread evenly across the middle of the aisle (all hoppable).
    std::vector<Rect> crates;
    crates.reserve(static_cast<std::size_t>(crateCount));
    const float span = lvl.worldWidth - 1200.0f;
    const float spacing = span / static_cast<float>(crateCount > 1 ? crateCount - 1 : 1);
    for (int c = 0; c < crateCount; ++c) {
        const float x = 620.0f + spacing * static_cast<float>(c);
        const float h = kCrateHeights[(i + c) % 4];
        crates.push_back({x, kFloorY - h, kCrateW, h});
    }
    lvl.crates = crates;

    // Toilet paper: first on the ground by the start, the rest on crate tops.
    lvl.tpSpawns.push_back({520.0f, kPickupY});
    for (int t = 1; t < tpGoal; ++t) {
        const Rect& c = crates[static_cast<std::size_t>((t - 1) % crateCount)];
        lvl.tpSpawns.push_back({c.x + 25.0f, c.y - 40.0f});
    }

    const int gapCount = crateCount - 1;  // >= 2
    const auto gapMid = [&](int k) {
        const auto& a = crates[static_cast<std::size_t>(k)];
        const auto& b = crates[static_cast<std::size_t>(k + 1)];
        return (a.right() + b.left()) * 0.5f;
    };

    // Sanitizers in the gaps between crates.
    const int sanCount = 1 + i / 3;
    for (int s = 0; s < sanCount; ++s) {
        lvl.sanitizerSpawns.push_back({gapMid(s % gapCount), kPickupY});
    }

    // Pushable carts in gaps (nudged so they don't sit on a pickup).
    const int cartCount = 1 + i / 2;
    for (int c = 0; c < cartCount; ++c) {
        lvl.cartSpawns.push_back({gapMid((c + i) % gapCount) - 20.0f, kCartY});
    }

    // Enemies in gaps: alternate patrollers and chasers.
    const int enemyCount = std::min(3 + i, 10);
    for (int e = 0; e < enemyCount; ++e) {
        const int k = e % gapCount;
        const float lo = crates[static_cast<std::size_t>(k)].right() + 10.0f;
        const float hi = crates[static_cast<std::size_t>(k + 1)].left() - 56.0f;
        const EnemyKind kind = (e % 2 == 0) ? EnemyKind::Patroller : EnemyKind::Chaser;
        const Vec2 pos{(lo + hi) * 0.5f, kEnemyY};
        lvl.enemySpawns.push_back({kind, pos, lo, hi});
    }

    return lvl;
}

}  // namespace

std::vector<Level> makeCampaign() {
    std::vector<Level> levels;
    levels.reserve(kLevelCount);
    for (int i = 0; i < kLevelCount; ++i) levels.push_back(makeLevel(i));
    return levels;
}

}  // namespace reclaim
