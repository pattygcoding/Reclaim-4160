#pragma once

#include <optional>
#include <string>
#include <vector>

#include "reclaim/core/Font.hpp"
#include "reclaim/core/Input.hpp"
#include "reclaim/core/Math.hpp"
#include "reclaim/core/Renderer.hpp"
#include "reclaim/ui/TextRenderer.hpp"

namespace reclaim {

// A vertical list of selectable options with keyboard navigation. Input handling
// is decoupled from audio, so callers know whether the selection moved (to play
// a blip) or was confirmed (to act on it).
class Menu {
public:
    struct Result {
        int selected = 0;
        bool moved = false;
        bool confirmed = false;
    };

    void clear() { labels_.clear(); selected_ = 0; }
    void add(std::string label) { labels_.push_back(std::move(label)); }

    [[nodiscard]] int selected() const noexcept { return selected_; }
    [[nodiscard]] int size() const noexcept { return static_cast<int>(labels_.size()); }
    [[nodiscard]] bool empty() const noexcept { return labels_.empty(); }
    void setSelected(int index) noexcept;

    // Navigate with W/S or Up/Down; confirm with Space/Enter.
    [[nodiscard]] Result update(const Input& input) noexcept;

    // Draw centred horizontally around centerX, beginning at topY.
    void draw(TextRenderer& text, Renderer& renderer, const Font& font,
              float centerX, float topY, float lineGap, Color normal, Color highlight,
              Color selectedText);
    void animate(float dt) noexcept { anim_ += dt; }

private:
    std::vector<std::string> labels_;
    int selected_ = 0;
    float anim_ = 0.0f;
};

}  // namespace reclaim
