#pragma once

#include <SDL2/SDL_mixer.h>

namespace reclaim {

// Thin, RAII owner of the SDL_mixer subsystem. Asset loading lives in the
// AssetManager; this class owns the mixer's global state (open/close, volumes,
// which music track is playing).
class Audio {
public:
    Audio() noexcept;
    ~Audio();

    Audio(const Audio&) = delete;
    Audio& operator=(const Audio&) = delete;

    [[nodiscard]] bool available() const noexcept { return available_; }

    void playSound(Mix_Chunk* chunk, int channel = -1, int loops = 0) const noexcept;
    // Plays a music track on a dedicated stream; ignores a request to restart
    // the track that is already playing.
    void playMusic(Mix_Music* music, int loops = -1) noexcept;
    void stopMusic() noexcept;
    void pauseMusic() noexcept;
    void resumeMusic() noexcept;

    void setMusicVolume(int volume) noexcept;   // 0..128
    void setSoundVolume(int volume) noexcept;   // 0..128

private:
    bool available_ = false;
    Mix_Music* current_ = nullptr;
    int musicVolume_ = 64;
    int soundVolume_ = 96;
};

}  // namespace reclaim
