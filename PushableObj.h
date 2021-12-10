#ifndef PUSHABLEPOBJ_H
#define PUSHABLEPOBJ_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameObject.h"

enum class PushableObjState { IDLE, PUSHED_FROM_LEFT, PUSHED_FROM_RIGHT };

class PushableObj: public GameObject{
    private:
        int pushForce;
        PushableObjState objState;
        PushableObjState LastMovementState;
        int PushedThisTick;
        double velocity;
    public:
        PushableObj();
        PushableObjState GetObjState();
        PushableObjState GetLastMovementState();
        double GetPushForce();
        double GetVelocity();
        int GetPushedtick();
        int GetSpeed();

        void SetPushedTick(int pushed); 
        void SetVelocity(int speed);
        void SetPushForce(int force);
        void SetObjState(PushableObjState state);

        //overrides
        void Update() override;
        void DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith) override;
        void SetIdle() override;
        std::string PrintObjType() override;

};

#endif
