#pragma once

#include <cstdint>
#include <random>

namespace reclaim {

// Small convenience wrapper around <random>. Never seeds from time() at each
// call site and never uses rand(); one engine owned by the app.
class Rng {
public:
    explicit Rng(std::uint32_t seed = std::random_device{}())
        : engine_(seed == 0 ? 0x9E3779B9u : seed) {}

    [[nodiscard]] float unit() { return std::uniform_real_distribution<float>(0.0f, 1.0f)(engine_); }
    [[nodiscard]] float range(float lo, float hi) {
        return std::uniform_real_distribution<float>(lo, hi)(engine_);
    }
    [[nodiscard]] int rangei(int lo, int hi) {
        if (hi < lo) std::swap(lo, hi);
        return std::uniform_int_distribution<int>(lo, hi)(engine_);
    }
    [[nodiscard]] bool chance(float probability) { return unit() < probability; }

private:
    std::mt19937 engine_;
};

}  // namespace reclaim
