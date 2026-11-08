#include "reclaim/core/Game.hpp"

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

namespace {

#if defined(__EMSCRIPTEN__)
reclaim::Game* g_game = nullptr;

void mainLoop() {
    if (g_game == nullptr || !g_game->isRunning()) {
        emscripten_cancel_main_loop();
        return;
    }
    g_game->frame();
}
#endif

}  // namespace

int main(int /*argc*/, char** /*argv*/) {
#if defined(__EMSCRIPTEN__)
    // The browser owns the loop: hand it a per-frame callback.
    static reclaim::Game game;
    g_game = &game;
    if (!game.start()) return 1;
    emscripten_set_main_loop(mainLoop, 0, 1);
    return 0;
#else
    reclaim::Game game;
    return game.run();
#endif
}
