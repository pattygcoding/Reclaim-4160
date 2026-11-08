#pragma once

#include <cstdint>

namespace reclaim {

// Frame timing: wall-clock delta (clamped so a hitch never tunnels entities),
// total elapsed seconds, and a frame counter.
class Time {
public:
    Time() noexcept;

    void tick() noexcept;

    [[nodiscard]] float delta() const noexcept { return delta_; }
    [[nodiscard]] double elapsed() const noexcept { return elapsed_; }
    [[nodiscard]] std::uint64_t frame() const noexcept { return frame_; }

private:
    std::uint64_t previous_;
    double  delta_ = 0.0;
    double  elapsed_ = 0.0;
    std::uint64_t frame_ = 0;
    double  maxDelta_ = 0.05;  // clamp to ~20 FPS minimum step
};

}  // namespace reclaim
