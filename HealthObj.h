#ifndef HEALTHOBJ_H
#define HEALTHOBJ_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "GameObject.h"
#include "ParticleEmitter.h"

enum class HealthObjState { NOT_COLLECTED, COLLECTED };
enum class HealthType { SANITIZER, WIPES };

class HealthObj: public GameObject{
    private:
        std::shared_ptr<ParticleEmitter> particleEmitter;
        HealthObjState objState;
        HealthType healthType;
        Mix_Chunk *healthSound;

    public:
        HealthObj();
        ~HealthObj();

        HealthObjState GetObjState();
        HealthType GetHealthType();
        std::shared_ptr<ParticleEmitter> GetParticleEmitter();

        void SetObjState(HealthObjState state);
        void SetHealthType(HealthType type);
        void ResetSprite();

        //overrides
        void Update() override;
        void Render() override;
        void DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith) override;
        std::string PrintObjType() override;
};

#endif
