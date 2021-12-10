#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum class ParticleType { RISE_FADEOUT };

class Particle {
    private:
        double XVel;
        double YVel;
        int lifetime;
        ParticleType particleType;

        SDL_Texture* graphic;
        SDL_Renderer *renderer;
        SDL_Rect graphic_rect;
    public:
        Particle();
        void Init(SDL_Renderer *ren, const char *file, int startX, int startY, int startW, int startH);
        void Update();
        void Render();

        void SetXVel(double v);
        void SetYVel(double v);
        void SetLifetime(int t);
        void SetParticleType(ParticleType type);

        double GetXVel();
        double GetYVel();
        int GetLifetime();
      
        ParticleType GetParticleType();

        
};

#endif