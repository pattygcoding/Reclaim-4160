#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "reclaim/core/Font.hpp"
#include "reclaim/core/Handles.hpp"
#include "reclaim/core/Texture.hpp"

namespace reclaim {

class Renderer;

// Central, lazily-loading cache for every game asset. Textures, fonts, sounds
// and music are each loaded at most once and referenced by handle. Handles are
// stable for the manager's lifetime because the underlying containers are
// node-based (pointer/iterator invalidation does not occur on insert).
class AssetManager {
public:
    explicit AssetManager(const Renderer& renderer) noexcept;

    // name without extension, e.g. texture("player") -> assets/images/player.png
    // Accessors are const: they lazily populate mutable caches.
    [[nodiscard]] const Texture* texture(std::string_view name) const;
    [[nodiscard]] const Font* font(std::string_view name, int pixelHeight) const;
    [[nodiscard]] Mix_Chunk* sound(std::string_view name) const;
    [[nodiscard]] Mix_Music* music(std::string_view name) const;

    // Loads a set of textures up front (avoids a hitch the first time each is
    // used). Errors are logged but never fatal.
    void warmup(std::initializer_list<std::string_view> textureNames) const;
    void clear();

private:
    const Renderer& renderer_;
    mutable std::unordered_map<std::string, Texture> textures_;
    mutable std::unordered_map<std::string, std::unique_ptr<Font>> fonts_;
    mutable std::unordered_map<std::string, ChunkPtr> sounds_;
    mutable std::unordered_map<std::string, MusicPtr> music_;
};

}  // namespace reclaim
