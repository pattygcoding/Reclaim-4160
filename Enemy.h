#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameObject.h"

enum class EnemyState { IDLE, MOVE_LEFT, MOVE_RIGHT};

class Enemy: public GameObject{
    private:
        int moveSpeed;
        EnemyState enemyState;
        double damagePerHit;
        bool aggro;
        int destination;

    public:
        Enemy();
        EnemyState GetEnemyState();
        int GetMovementSpeed();
        double GetDamagePerHit();
        bool GetAggro();
        int GetDestination();

        void SetMovementSpeed(int speed);
        void SetEnemyState(EnemyState state);
        void SetDamagePerHit(double d);
        void SetAggro(bool a);
        void SetDestination(int d);

        int UpdateFrame(int frame_duration, int start_frame, int totalFrameCount);

        //overrides
        void Update() override;
        void DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith) override;
        void SetIdle() override;
        std::string PrintObjType() override;

};

#endif
