#include "reclaim/core/Audio.hpp"

#include <SDL2/SDL.h>

#include <cstdio>

namespace reclaim {

Audio::Audio() noexcept {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::fprintf(stderr, "[audio] SDL_mixer init failed: %s\n", Mix_GetError());
        available_ = false;
        return;
    }
    available_ = true;
    Mix_AllocateChannels(16);
    Mix_VolumeMusic(musicVolume_);
    Mix_Volume(-1, soundVolume_);
}

Audio::~Audio() {
    if (available_) {
        Mix_HaltMusic();
        Mix_CloseAudio();
    }
}

void Audio::playSound(Mix_Chunk* chunk, int channel, int loops) const noexcept {
    if (available_ && chunk) Mix_PlayChannel(channel, chunk, loops);
}

void Audio::playMusic(Mix_Music* music, int loops) noexcept {
    if (!available_ || !music) return;
    if (current_ == music && Mix_PlayingMusic()) return;  // already playing
    Mix_HaltMusic();
    if (Mix_PlayMusic(music, loops) == 0) current_ = music;
}

void Audio::stopMusic() noexcept {
    if (!available_) return;
    Mix_HaltMusic();
    current_ = nullptr;
}

void Audio::pauseMusic() noexcept {
    if (available_) Mix_PauseMusic();
}

void Audio::resumeMusic() noexcept {
    if (available_) Mix_ResumeMusic();
}

void Audio::setMusicVolume(int volume) noexcept {
    musicVolume_ = volume;
    if (available_) Mix_VolumeMusic(volume);
}

void Audio::setSoundVolume(int volume) noexcept {
    soundVolume_ = volume;
    if (available_) Mix_Volume(-1, volume);
}

}  // namespace reclaim
