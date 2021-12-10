#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <memory>

#include "Particle.h"

class ParticleEmitter {
    private:
        std::vector<std::shared_ptr<Particle>> particles;
        ParticleType particleType;
        int duration;
    public:
        ParticleEmitter();

        void Init(SDL_Renderer *ren, const char *file, int startX, int startY, int maxOffsetX, int maxOffsetY, int particleW, int particleH, ParticleType type);
        void Update();
        void Render();
        void Quit();

        int GetDuration();
};

#endif