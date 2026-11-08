#pragma once

#include <memory>
#include <vector>

#include "reclaim/core/Math.hpp"
#include "reclaim/gfx/ParticleSystem.hpp"
#include "reclaim/scenes/Scene.hpp"
#include "reclaim/ui/Hud.hpp"
#include "reclaim/ui/Menu.hpp"
#include "reclaim/world/Checkout.hpp"
#include "reclaim/world/Enemy.hpp"
#include "reclaim/world/Level.hpp"
#include "reclaim/world/Pickup.hpp"
#include "reclaim/world/Player.hpp"
#include "reclaim/world/Pushable.hpp"

namespace reclaim {

class AssetManager;
class Audio;

// The actual game. Owns the entities for one level, runs the update/collision
// loop, scrolls the camera, draws the HUD, and drives the pause/win/lose flow.
class PlayScene final : public Scene {
public:
    PlayScene(Game& game, int levelIndex);

    void enter() override;
    void update(float dt) override;
    void render() override;

    // --- Services used by entities (keeps entity code free of Game details) ---
    [[nodiscard]] const Input& input() const noexcept;
    [[nodiscard]] const std::vector<Rect>& solidRects() const noexcept { return solids_; }
    [[nodiscard]] float worldWidth() const noexcept { return level_.worldWidth; }
    [[nodiscard]] float floorY() const noexcept { return level_.floorY; }
    [[nodiscard]] Rect playerBounds() const noexcept;
    [[nodiscard]] Vec2 playerCenter() const noexcept;
    [[nodiscard]] bool playerAlive() const noexcept;
    [[nodiscard]] Rng& rng() noexcept;
    [[nodiscard]] ParticleSystem& particles() noexcept { return particles_; }
    [[nodiscard]] AssetManager& assets() noexcept;
    [[nodiscard]] Audio& audio() noexcept;

    // --- Gameplay callbacks (entities -> scene) ---
    void addScore(int amount) noexcept { score_ += amount; }
    void shakeCamera(float amount) noexcept;
    void collectPickup(Pickup& pickup);
    void defeatEnemy(Enemy& enemy);
    void winLevel();
    void loseLevel();

private:
    enum class Phase { Playing, Paused, Winning, Losing };

    void buildLevel();
    void updatePlaying(float dt);
    void resolvePlayerInteractions();
    void updateCamera(float dt);
    void handleTransitions(float dt);
    void renderWorld();
    void renderHud();
    void renderOverlay();
    void renderDimBackdrop(Color tint);
    [[nodiscard]] HudData makeHudData() const;

    Level level_;
    int levelIndex_;
    std::vector<Rect> solids_;

    std::unique_ptr<Player> player_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<Pickup>> pickups_;
    std::vector<std::unique_ptr<Pushable>> carts_;
    std::unique_ptr<Checkout> checkout_;

    ParticleSystem particles_;
    Hud hud_;
    Menu pauseMenu_;

    Phase phase_ = Phase::Playing;
    float phaseTimer_ = 0.0f;
    float cameraX_ = 0.0f;
    float shake_ = 0.0f;
    float shakeSeed_ = 0.0f;
    float elapsed_ = 0.0f;
    float timeRemaining_ = -1.0f;
    float hurtFlash_ = 0.0f;
    int   score_ = 0;
    int   tpCollected_ = 0;
    int   tpGoal_ = 1;
    bool  checkoutOpen_ = false;
    bool  loseByTime_ = false;
};

}  // namespace reclaim
