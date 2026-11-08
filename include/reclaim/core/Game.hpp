#pragma once

#include <memory>

#include "reclaim/core/Assets.hpp"
#include "reclaim/core/Audio.hpp"
#include "reclaim/core/Handles.hpp"
#include "reclaim/core/Input.hpp"
#include "reclaim/core/Random.hpp"
#include "reclaim/core/Renderer.hpp"
#include "reclaim/core/Time.hpp"
#include "reclaim/scenes/Scene.hpp"
#include "reclaim/ui/TextRenderer.hpp"

namespace reclaim {

// The application root: owns the window, renderer and all shared services, runs
// the main loop, and manages the active Scene. Every other object reaches the
// services it needs through the Game it is handed, so there are no globals and
// no hidden singletons.
class Game {
public:
    Game() noexcept = default;
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    // Native entry point: initialises, runs the loop until quit, then shuts down.
    int run();

    // Step-based API used by the Emscripten main loop (browser-owned loop).
    // start() initialises and enters the first scene; frame() advances one tick.
    bool start();
    void frame();
    void stop();
    [[nodiscard]] bool isRunning() const noexcept { return running_; }

    // --- Shared services ---
    [[nodiscard]] Renderer&      renderer() noexcept { return *renderer_; }
    [[nodiscard]] const Renderer& view() const noexcept { return *renderer_; }
    [[nodiscard]] Input&         input() noexcept { return input_; }
    [[nodiscard]] AssetManager&  assets() noexcept { return *assets_; }
    [[nodiscard]] Audio&         audio() noexcept { return *audio_; }
    [[nodiscard]] TextRenderer&  text() noexcept { return *text_; }
    [[nodiscard]] Time&          time() noexcept { return time_; }
    [[nodiscard]] Rng&           rng() noexcept { return rng_; }

    [[nodiscard]] int width()  const noexcept { return kWidth; }
    [[nodiscard]] int height() const noexcept { return kHeight; }

    // --- Meta / session state ---
    struct Session {
        int level = 1;       // 1-based current level
        int score = 0;       // accumulated this run
        int bestScore = 0;
    };
    [[nodiscard]] Session& session() noexcept { return session_; }

    void requestScene(std::unique_ptr<Scene> scene) noexcept { pending_ = std::move(scene); }
    void quit() noexcept { running_ = false; }

    static constexpr int kWidth = 960;
    static constexpr int kHeight = 540;

private:
    bool initialize();
    void shutdown();
    void processEvents();
    void updateActiveScene(float dt);
    void renderFrame();
    void applyPendingScene();

    WindowPtr window_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<AssetManager> assets_;
    std::unique_ptr<Audio> audio_;
    std::unique_ptr<TextRenderer> text_;

    Input input_;
    Time time_;
    Rng rng_;
    Session session_;

    std::unique_ptr<Scene> scene_;
    std::unique_ptr<Scene> pending_;
    bool running_ = false;
};

}  // namespace reclaim
