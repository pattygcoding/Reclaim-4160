#pragma once

#include <SDL2/SDL.h>

#include <array>

namespace reclaim {

// Tracks keyboard state with edge detection (down / just-pressed / released).
// The application drives it: call beginFrame() once per frame, then feed every
// polled SDL_Event through handleEvent().
class Input {
public:
    Input() noexcept;

    void beginFrame() noexcept;
    void handleEvent(const SDL_Event& event) noexcept;

    [[nodiscard]] bool isDown(SDL_Scancode key) const noexcept;
    [[nodiscard]] bool justPressed(SDL_Scancode key) const noexcept;
    [[nodiscard]] bool justReleased(SDL_Scancode key) const noexcept;

    // -1, 0 or +1 depending on which of the two keys is held.
    [[nodiscard]] float axis(SDL_Scancode negative, SDL_Scancode positive) const noexcept;

    void reset() noexcept;

private:
    std::array<bool, SDL_NUM_SCANCODES> current_{};
    std::array<bool, SDL_NUM_SCANCODES> previous_{};
};

}  // namespace reclaim
