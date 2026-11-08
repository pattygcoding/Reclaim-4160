#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "reclaim/core/Font.hpp"
#include "reclaim/core/Math.hpp"
#include "reclaim/core/Renderer.hpp"
#include "reclaim/core/Texture.hpp"

namespace reclaim {

// Renders UTF-8 text through SDL2_ttf, caching the resulting textures keyed by
// (font, text, colour) so per-frame strings cost only a hash lookup. All draw
// calls honour the Renderer's camera offset, so callers reset the camera for UI.
class TextRenderer {
public:
    enum class Align { Left, Center, Right };

    explicit TextRenderer(Renderer& renderer) noexcept : renderer_(renderer) {}

    void draw(const Font& font, std::string_view text, Vec2 position, Color color,
              Align align = Align::Left);
    void drawShadowed(const Font& font, std::string_view text, Vec2 position, Color color,
                      Color shadow, Vec2 shadowOffset, Align align = Align::Left);
    void drawScaled(const Font& font, std::string_view text, Vec2 position, Color color,
                    float scale, Align align = Align::Left);

    [[nodiscard]] Vec2 measure(const Font& font, std::string_view text) const noexcept;

    void clearCache() { cache_.clear(); }
    void pruneIfLarge();

private:
    [[nodiscard]] const Texture* textureFor(const Font& font, std::string_view text, Color color);
    void drawTexture(const Texture& tex, Vec2 position, float scale, Align align);

    Renderer& renderer_;
    std::unordered_map<std::string, Texture> cache_;
};

}  // namespace reclaim
