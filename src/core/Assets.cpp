#include "reclaim/core/Assets.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <cstdio>
#include <memory>
#include <string>

#include "reclaim/core/Paths.hpp"
#include "reclaim/core/Renderer.hpp"

namespace reclaim {

AssetManager::AssetManager(const Renderer& renderer) noexcept : renderer_(renderer) {}

const Texture* AssetManager::texture(std::string_view name) const {
    const std::string key{name};
    if (const auto it = textures_.find(key); it != textures_.end()) return &it->second;

    const std::string path = paths::image(key);
    SDL_Texture* raw = IMG_LoadTexture(renderer_.raw(), path.c_str());
    if (!raw) {
        std::fprintf(stderr, "[assets] texture load failed: %s (%s)\n", path.c_str(), IMG_GetError());
        return nullptr;
    }
    int w = 0;
    int h = 0;
    SDL_QueryTexture(raw, nullptr, nullptr, &w, &h);
    SDL_SetTextureBlendMode(raw, SDL_BLENDMODE_BLEND);
    const auto result = textures_.emplace(key, Texture(TexturePtr(raw), w, h));
    return &result.first->second;
}

const Font* AssetManager::font(std::string_view name, int pixelHeight) const {
    const std::string key = std::string(name) + "#" + std::to_string(pixelHeight);
    if (const auto it = fonts_.find(key); it != fonts_.end()) return it->second.get();

    const std::string path = paths::font(name);
    TTF_Font* raw = TTF_OpenFont(path.c_str(), pixelHeight);
    if (!raw) {
        std::fprintf(stderr, "[assets] font load failed: %s (%s)\n", path.c_str(), TTF_GetError());
        return nullptr;
    }
    auto owned = std::make_unique<Font>(FontPtr(raw), pixelHeight);
    const Font* ptr = owned.get();
    fonts_.emplace(key, std::move(owned));
    return ptr;
}

Mix_Chunk* AssetManager::sound(std::string_view name) const {
    const std::string key{name};
    if (const auto it = sounds_.find(key); it != sounds_.end()) return it->second.get();

    const std::string path = paths::sound(key);
    Mix_Chunk* raw = Mix_LoadWAV(path.c_str());
    if (!raw) {
        std::fprintf(stderr, "[assets] sound load failed: %s (%s)\n", path.c_str(), Mix_GetError());
        return nullptr;
    }
    Mix_Chunk* ptr = raw;
    sounds_.emplace(key, ChunkPtr(raw));
    return ptr;
}

Mix_Music* AssetManager::music(std::string_view name) const {
    const std::string key{name};
    if (const auto it = music_.find(key); it != music_.end()) return it->second.get();

    const std::string path = paths::sound(key);
    Mix_Music* raw = Mix_LoadMUS(path.c_str());
    if (!raw) {
        std::fprintf(stderr, "[assets] music load failed: %s (%s)\n", path.c_str(), Mix_GetError());
        return nullptr;
    }
    Mix_Music* ptr = raw;
    music_.emplace(key, MusicPtr(raw));
    return ptr;
}

void AssetManager::warmup(std::initializer_list<std::string_view> textureNames) const {
    for (const std::string_view name : textureNames) (void)texture(name);
}

void AssetManager::clear() {
    textures_.clear();
    fonts_.clear();
    sounds_.clear();
    music_.clear();
}

}  // namespace reclaim
