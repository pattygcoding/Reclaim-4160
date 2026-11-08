#include "reclaim/world/Level.hpp"

namespace reclaim {

namespace {
constexpr float kFloorY = 470.0f;
constexpr float kEnemyY = kFloorY - 46.0f;   // enemies are 46 tall
constexpr float kPickupY = kFloorY - 84.0f;
constexpr float kCartY = kFloorY - 56.0f;    // cart is 56 tall
constexpr float kCheckoutY = kFloorY - 104.0f;

EnemySpawn patroller(Vec2 pos, float lo, float hi) { return {EnemyKind::Patroller, pos, lo, hi}; }
EnemySpawn chaser(Vec2 pos, float lo, float hi) { return {EnemyKind::Chaser, pos, lo, hi}; }
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

        lvl.crates = {
            {620.0f, 372.0f, 120.0f, 98.0f},
            {1120.0f, 300.0f, 150.0f, 170.0f},
            {1600.0f, 392.0f, 110.0f, 78.0f},
            {1980.0f, 330.0f, 150.0f, 140.0f},
        };
        lvl.tpSpawns = {
            {500.0f, kPickupY}, {1420.0f, kFloorY - 250.0f}, {2100.0f, kPickupY},
        };
        lvl.sanitizerSpawns = {{1180.0f, kFloorY - 260.0f}};
        lvl.cartSpawns = {{900.0f, kCartY}};
        lvl.enemySpawns = {
            patroller({720.0f, kEnemyY}, 620.0f, 1080.0f),
            chaser({1450.0f, kEnemyY}, 1250.0f, 1900.0f),
            patroller({2050.0f, kEnemyY}, 1980.0f, 2400.0f),
        };
        lvl.playerStart = {90.0f, kFloorY - 70.0f};
        lvl.checkoutPos = {2450.0f, kCheckoutY};
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

        lvl.crates = {
            {520.0f, 360.0f, 110.0f, 110.0f},
            {980.0f, 300.0f, 140.0f, 170.0f},
            {1450.0f, 380.0f, 120.0f, 90.0f},
            {1900.0f, 320.0f, 160.0f, 150.0f},
            {2400.0f, 380.0f, 120.0f, 90.0f},
            {2750.0f, 300.0f, 150.0f, 170.0f},
        };
        lvl.tpSpawns = {
            {430.0f, kPickupY}, {1200.0f, kFloorY - 250.0f},
            {2080.0f, kFloorY - 260.0f}, {2900.0f, kFloorY - 260.0f},
        };
        lvl.sanitizerSpawns = {{1000.0f, kFloorY - 260.0f}, {2350.0f, kPickupY}};
        lvl.cartSpawns = {{760.0f, kCartY}, {2200.0f, kCartY}};
        lvl.enemySpawns = {
            patroller({600.0f, kEnemyY}, 520.0f, 940.0f),
            chaser({1300.0f, kEnemyY}, 1000.0f, 1650.0f),
            patroller({1850.0f, kEnemyY}, 1650.0f, 2200.0f),
            chaser({2500.0f, kEnemyY}, 2200.0f, 3000.0f),
        };
        lvl.playerStart = {70.0f, kFloorY - 70.0f};
        lvl.checkoutPos = {3160.0f, kCheckoutY};
        lvl.tpGoal = 4;
        lvl.timeLimit = 120.0f;
        levels.push_back(std::move(lvl));
    }

    return levels;
}

}  // namespace reclaim
