#ifndef TOILETPAPER_H
#define TOILETPAPER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameObject.h"

enum class TPObjState { NOT_COLLECTED, COLLECTED };

class ToiletPaper: public GameObject{
    private:
        TPObjState objState;
        int fadeDuration;

    public:
        ToiletPaper();
        ~ToiletPaper();

        TPObjState GetObjState();

        void SetObjState(TPObjState state);
        void ResetSprite();

        //overrides
        void Update() override;
        void Render() override;
        void DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith) override;
        std::string PrintObjType() override;
};

#endif
