#include "reclaim/ui/Hud.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>

#include "reclaim/core/Assets.hpp"
#include "reclaim/core/Texture.hpp"
#include "reclaim/ui/Hud.hpp"

namespace reclaim {

void Hud::draw(Renderer& r, TextRenderer& t, const Font& font, AssetManager& assets,
               const HudData& d, int screenWidth) const {
    r.resetCamera();  // HUD is always drawn in screen space
    const float pad = 18.0f;

    // ---- Health panel (top-left) ----
    {
        const Rect panel{pad, pad, 300.0f, 46.0f};
        r.fillRoundedRect(panel, 14.0f, Color::rgba(0, 0, 0, 110));

        const float iconSize = 28.0f;
        if (const Texture* heart = assets.texture("icon_heart"); heart && heart->valid()) {
            r.drawTexture(*heart, {panel.x + 10.0f, panel.cy() - iconSize * 0.5f, iconSize, iconSize});
        }

        const float barX = panel.x + 10.0f + iconSize + 8.0f;
        const float barW = panel.right() - barX - 12.0f;
        const float barH = 16.0f;
        const Rect barBg{barX, panel.cy() - barH * 0.5f, barW, barH};
        r.fillRoundedRect(barBg, barH * 0.5f, Color::rgba(255, 255, 255, 40));

        const float ratio = d.maxHealth > 0.0f ? std::clamp(d.health / d.maxHealth, 0.0f, 1.0f) : 0.0f;
        const Color fill = ratio > 0.5f ? Color::rgb(96, 200, 120)
                          : ratio > 0.25f ? Color::rgb(240, 180, 60)
                                          : Color::rgb(220, 70, 70);
        if (ratio > 0.0f) {
            const Rect fillRect{barBg.x, barBg.y, std::max(barH, barBg.w * ratio), barH};
            r.fillRoundedRect(fillRect, barH * 0.5f, fill);
        }

        if (d.hurtFlash > 0.01f) {
            const auto a = static_cast<std::uint8_t>(140.0f * std::clamp(d.hurtFlash, 0.0f, 1.0f));
            r.drawRoundedRect(panel.inflated(2.0f), 16.0f, Color::rgba(255, 80, 80, a));
        }
    }

    // ---- Toilet-paper count (below health) ----
    {
        const Rect panel{pad, pad + 46.0f + 10.0f, 168.0f, 40.0f};
        r.fillRoundedRect(panel, 12.0f, Color::rgba(0, 0, 0, 110));

        const float iconSize = 26.0f;
        if (const Texture* tp = assets.texture("tp"); tp && tp->valid()) {
            r.drawTexture(*tp, {panel.x + 10.0f, panel.cy() - iconSize * 0.5f, iconSize, iconSize});
        }

        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "%d / %d", d.tpCollected, d.tpGoal);
        t.draw(font, buffer, {panel.x + 10.0f + iconSize + 10.0f, panel.cy() - font.pixelHeight() * 0.5f},
               Color::Cream, TextRenderer::Align::Left);
    }

    // ---- Score (top-right) ----
    {
        char buffer[48];
        std::snprintf(buffer, sizeof(buffer), "SCORE %d", d.score);
        const Vec2 measured = t.measure(font, buffer);
        const Vec2 pos{static_cast<float>(screenWidth) - pad - measured.x, pad + 4.0f};
        t.drawShadowed(font, buffer, pos, Color::Cream, Color::rgba(0, 0, 0, 130), {2.0f, 2.0f});
    }

    // ---- Level name + timer (top-centre) ----
    {
        const float cx = static_cast<float>(screenWidth) * 0.5f;
        t.drawShadowed(font, d.levelName, {cx, pad}, Color::Cream, Color::rgba(0, 0, 0, 130), {2.0f, 2.0f},
                       TextRenderer::Align::Center);
        if (d.timeRemaining >= 0.0f) {
            char buffer[32];
            const int secs = static_cast<int>(std::ceil(d.timeRemaining));
            std::snprintf(buffer, sizeof(buffer), "%d:%02d", secs / 60, secs % 60);
            const Color tc = d.timeRemaining < 10.0f ? Color::rgb(255, 120, 110) : Color::Cream;
            t.drawShadowed(font, buffer, {cx, pad + static_cast<float>(font.pixelHeight()) + 6.0f},
                           tc, Color::rgba(0, 0, 0, 130), {2.0f, 2.0f}, TextRenderer::Align::Center);
        }
    }
}

}  // namespace reclaim
