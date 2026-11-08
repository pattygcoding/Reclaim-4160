#include "reclaim/core/Time.hpp"

#include <SDL2/SDL.h>

namespace reclaim {

Time::Time() noexcept : previous_(SDL_GetPerformanceCounter()) {}

void Time::tick() noexcept {
    const std::uint64_t now = SDL_GetPerformanceCounter();
    const std::uint64_t freq = SDL_GetPerformanceFrequency();
    double dt = freq != 0 ? static_cast<double>(now - previous_) / static_cast<double>(freq) : 0.0;
    previous_ = now;

    if (dt < 0.0) dt = 0.0;
    if (dt > maxDelta_) dt = maxDelta_;  // clamp to avoid tunnelling after a hitch

    delta_ = dt;
    elapsed_ += dt;
    ++frame_;
}

}  // namespace reclaim
