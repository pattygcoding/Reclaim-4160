#include "reclaim/core/Input.hpp"

namespace reclaim {

Input::Input() noexcept { reset(); }

void Input::reset() noexcept {
    current_.fill(false);
    previous_.fill(false);
}

void Input::beginFrame() noexcept { previous_ = current_; }

void Input::handleEvent(const SDL_Event& event) noexcept {
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        const SDL_Scancode sc = event.key.keysym.scancode;
        if (sc > SDL_SCANCODE_UNKNOWN && sc < SDL_NUM_SCANCODES) {
            current_[sc] = (event.type == SDL_KEYDOWN);
        }
    }
}

bool Input::isDown(SDL_Scancode key) const noexcept { return current_[key]; }

bool Input::justPressed(SDL_Scancode key) const noexcept {
    return current_[key] && !previous_[key];
}

bool Input::justReleased(SDL_Scancode key) const noexcept {
    return !current_[key] && previous_[key];
}

float Input::axis(SDL_Scancode negative, SDL_Scancode positive) const noexcept {
    return (isDown(positive) ? 1.0f : 0.0f) - (isDown(negative) ? 1.0f : 0.0f);
}

}  // namespace reclaim
