#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace reclaim {

// ---------------------------------------------------------------------------
//  Small, header-only 2D math helpers used across the engine.
//  Deliberately dependency-free (no SDL) so it stays unit-testable.
// ---------------------------------------------------------------------------

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    constexpr Vec2() = default;
    constexpr Vec2(float x_, float y_) : x(x_), y(y_) {}

    [[nodiscard]] constexpr Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    [[nodiscard]] constexpr Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    [[nodiscard]] constexpr Vec2 operator*(float s) const { return {x * s, y * s}; }
    [[nodiscard]] constexpr Vec2 operator/(float s) const { return {x / s, y / s}; }

    constexpr Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    constexpr Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }

    [[nodiscard]] float lengthSq() const { return x * x + y * y; }
    [[nodiscard]] float length() const { return std::sqrt(lengthSq()); }

    [[nodiscard]] Vec2 normalized() const {
        const float len = length();
        return len > 1e-6f ? Vec2{x / len, y / len} : Vec2{};
    }

    [[nodiscard]] static constexpr float dot(const Vec2& a, const Vec2& b) {
        return a.x * b.x + a.y * b.y;
    }
};

// Axis-aligned rectangle in world/screen space.
struct Rect {
    float x = 0.0f;
    float y = 0.0f;
    float w = 0.0f;
    float h = 0.0f;

    [[nodiscard]] constexpr float left()   const { return x; }
    [[nodiscard]] constexpr float top()    const { return y; }
    [[nodiscard]] constexpr float right()  const { return x + w; }
    [[nodiscard]] constexpr float bottom() const { return y + h; }
    [[nodiscard]] constexpr Vec2  center() const { return {x + w * 0.5f, y + h * 0.5f}; }
    [[nodiscard]] constexpr float cx()     const { return x + w * 0.5f; }
    [[nodiscard]] constexpr float cy()     const { return y + h * 0.5f; }

    [[nodiscard]] constexpr bool intersects(const Rect& o) const {
        return left() < o.right() && right() > o.left() &&
               top() < o.bottom() && bottom() > o.top();
    }

    [[nodiscard]] constexpr bool contains(Vec2 p) const {
        return p.x >= left() && p.x <= right() && p.y >= top() && p.y <= bottom();
    }

    [[nodiscard]] constexpr Rect inflated(float m) const {
        return {x - m, y - m, w + 2.0f * m, h + 2.0f * m};
    }

    [[nodiscard]] constexpr Rect translated(float dx, float dy) const {
        return {x + dx, y + dy, w, h};
    }
};

// 8-bit RGBA colour. Integer channels match SDL_SetRenderDrawColor directly.
struct Color {
    std::uint8_t r = 255;
    std::uint8_t g = 255;
    std::uint8_t b = 255;
    std::uint8_t a = 255;

    [[nodiscard]] constexpr Color withAlpha(std::uint8_t alpha) const { return {r, g, b, alpha}; }

    [[nodiscard]] static constexpr Color rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
        return {r, g, b, 255};
    }
    [[nodiscard]] static constexpr Color rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) {
        return {r, g, b, a};
    }

    // Palette colours. Declared here, defined out-of-line below so that the
    // type is complete before these values are initialised.
    static const Color White;
    static const Color Black;
    static const Color Ink;
    static const Color Cream;
    static const Color Mint;
    static const Color Teal;
    static const Color Coral;
    static const Color Amber;
    static const Color Purple;
    static const Color Red;
};

inline const Color Color::White  = Color::rgb(255, 255, 255);
inline const Color Color::Black  = Color::rgb(20, 18, 28);
inline const Color Color::Ink    = Color::rgb(34, 30, 48);
inline const Color Color::Cream  = Color::rgb(255, 246, 230);
inline const Color Color::Mint   = Color::rgb(107, 214, 176);
inline const Color Color::Teal   = Color::rgb(38, 166, 154);
inline const Color Color::Coral  = Color::rgb(255, 111, 97);
inline const Color Color::Amber  = Color::rgb(255, 199, 89);
inline const Color Color::Purple = Color::rgb(124, 92, 191);
inline const Color Color::Red    = Color::rgb(214, 69, 69);

// ---------------------------------------------------------------------------
//  Scalar utilities
// ---------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr T clamp(T v, T lo, T hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

[[nodiscard]] constexpr float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// Frame-rate independent smoothing: fraction of the way toward target per second.
[[nodiscard]] inline float damp(float current, float target, float rate, float dt) {
    return lerp(current, target, 1.0f - std::exp(-rate * dt));
}

[[nodiscard]] inline float approach(float current, float target, float maxDelta) {
    if (current < target) return std::min(current + maxDelta, target);
    return std::max(current - maxDelta, target);
}

[[nodiscard]] inline float sign(float v) { return (v > 0.0f) - (v < 0.0f); }

// Easing curves for juicy UI/game feel.
namespace ease {
[[nodiscard]] inline float outCubic(float t) {
    const float f = 1.0f - t;
    return 1.0f - f * f * f;
}
[[nodiscard]] inline float outBack(float t) {
    constexpr float c1 = 1.70158f;
    constexpr float c3 = c1 + 1.0f;
    const float f = t - 1.0f;
    return 1.0f + c3 * f * f * f + c1 * f * f;
}
[[nodiscard]] inline float inOutSine(float t) {
    return -(std::cos(3.14159265f * t) - 1.0f) * 0.5f;
}
}  // namespace ease

}  // namespace reclaim
