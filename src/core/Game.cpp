#include "reclaim/core/Game.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <cstdio>
#include <memory>

#include "reclaim/scenes/TitleScene.hpp"

namespace reclaim {

Game::~Game() { shutdown(); }

bool Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
        std::fprintf(stderr, "[game] SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        std::fprintf(stderr, "[game] IMG_Init failed: %s\n", IMG_GetError());
    }
    if (TTF_Init() != 0) {
        std::fprintf(stderr, "[game] TTF_Init failed: %s\n", TTF_GetError());
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    window_.reset(SDL_CreateWindow("Reclaim-4160", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   kWidth, kHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));
    if (!window_) {
        std::fprintf(stderr, "[game] CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_Renderer* raw = SDL_CreateRenderer(window_.get(), -1,
                                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!raw) raw = SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED);
    if (!raw) raw = SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_SOFTWARE);
    if (!raw) {
        std::fprintf(stderr, "[game] CreateRenderer failed: %s\n", SDL_GetError());
        return false;
    }
    SDL_RenderSetLogicalSize(raw, kWidth, kHeight);

    renderer_ = std::make_unique<Renderer>(RendererPtr(raw), kWidth, kHeight);
    assets_ = std::make_unique<AssetManager>(*renderer_);
    audio_ = std::make_unique<Audio>();
    text_ = std::make_unique<TextRenderer>(*renderer_);
    return true;
}

void Game::shutdown() {
    scene_.reset();
    pending_.reset();
    text_.reset();
    assets_.reset();
    audio_.reset();
    renderer_.reset();
    window_.reset();
    if (TTF_WasInit()) TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Game::start() {
    if (!initialize()) return false;
    scene_ = std::make_unique<TitleScene>(*this);
    if (scene_) scene_->enter();
    running_ = true;
    return true;
}

void Game::frame() {
    if (!running_) return;

    time_.tick();
    input_.beginFrame();
    processEvents();
    if (!running_) return;

    if (scene_) scene_->update(time_.delta());
    applyPendingScene();

    if (renderer_) {
        renderer_->beginFrame(Color::rgb(18, 20, 28));
        if (scene_) scene_->render();
        renderer_->endFrame();
    }

    // Keep the text cache from growing without bound over a long session.
    if (text_) text_->pruneIfLarge();
}

void Game::stop() {
    running_ = false;
    shutdown();
}

int Game::run() {
    if (!start()) {
        shutdown();
        return 1;
    }
    while (running_) frame();
    stop();
    return 0;
}

void Game::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running_ = false;
        input_.handleEvent(event);
    }
}

void Game::applyPendingScene() {
    if (!pending_) return;
    if (scene_) scene_->exit();
    scene_ = std::move(pending_);
    if (scene_) scene_->enter();
}

}  // namespace reclaim
