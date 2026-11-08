#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>

namespace reclaim {

// ---------------------------------------------------------------------------
//  RAII ownership for SDL C resources via std::unique_ptr with custom deleters.
//  No SDL_* object is ever leaked or double-freed: ownership is explicit in the
//  type system. Prefer these handles over raw pointers everywhere.
// ---------------------------------------------------------------------------

struct WindowDeleter {
    void operator()(SDL_Window* w) const noexcept { if (w) SDL_DestroyWindow(w); }
};
struct RendererDeleter {
    void operator()(SDL_Renderer* r) const noexcept { if (r) SDL_DestroyRenderer(r); }
};
struct TextureDeleter {
    void operator()(SDL_Texture* t) const noexcept { if (t) SDL_DestroyTexture(t); }
};
struct SurfaceDeleter {
    void operator()(SDL_Surface* s) const noexcept { if (s) SDL_FreeSurface(s); }
};
struct FontDeleter {
    void operator()(TTF_Font* f) const noexcept { if (f) TTF_CloseFont(f); }
};
struct ChunkDeleter {
    void operator()(Mix_Chunk* c) const noexcept { if (c) Mix_FreeChunk(c); }
};
struct MusicDeleter {
    void operator()(Mix_Music* m) const noexcept { if (m) Mix_FreeMusic(m); }
};

using WindowPtr   = std::unique_ptr<SDL_Window, WindowDeleter>;
using RendererPtr = std::unique_ptr<SDL_Renderer, RendererDeleter>;
using TexturePtr  = std::unique_ptr<SDL_Texture, TextureDeleter>;
using SurfacePtr  = std::unique_ptr<SDL_Surface, SurfaceDeleter>;
using FontPtr     = std::unique_ptr<TTF_Font, FontDeleter>;
using ChunkPtr    = std::unique_ptr<Mix_Chunk, ChunkDeleter>;
using MusicPtr    = std::unique_ptr<Mix_Music, MusicDeleter>;

}  // namespace reclaim
