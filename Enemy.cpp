
#include "Enemy.h"
#include "GameEngine.h"

#include <string>
#include <SDL2/SDL_image.h>
#include <iostream>

Enemy::Enemy()
{
    objType = ObjType::Enemy;
    enemyState = EnemyState::IDLE;
    moveSpeed = 2;
    damagePerHit = 0.2;
    aggro = false;
    srand(time(NULL));
    destination = rand() % 1000;
};

GameEngine* game4 = GameEngine::get_instance();

EnemyState Enemy::GetEnemyState(){return enemyState;}
int Enemy::GetMovementSpeed(){return moveSpeed;}
double Enemy::GetDamagePerHit(){return damagePerHit;}
bool Enemy::GetAggro(){return aggro;}
int Enemy::GetDestination(){return destination;}


/*
    state = one of the valid states that Enemy can have. Valid states (defined in header file) are IDLE, MOVE_LEFT, MOVE_RIGHT, JUMP, FALL
    (jump state can be set but it doesn't do anything yet in this build)
*/
void Enemy::SetEnemyState(EnemyState state){enemyState = state;}
void Enemy::SetMovementSpeed(int speed){moveSpeed = speed;}
void Enemy::SetDamagePerHit(double d){damagePerHit = d;}
void Enemy::SetAggro(bool a){aggro = a;}
void Enemy::SetDestination(int d){destination = d;}



//Updates Enemy's position and sprite animation frame depending on Enemy's state
void Enemy::Update(){
    int moveAnimYOffset = GetSprite()->GetSrcH(); //used to set sprite's src_rect to the next row of sprites in the sprite sheet
    int moveAnimTotalFrames = 4;
    int moveAnimStartFrame = 0; //which animation frame you want to start at

    int animSpeed = 200;

    int idleAnimTotalFrames = 4;
    int idleAnimStartFrame = 0;

    //link collider to Enemy pos
    SetBoxColliderPos(GetSprite()->GetX(), GetSprite()->GetY());
    
    switch(enemyState){
        case EnemyState::IDLE: {
            GetSprite()->SetSrcY(0);
            UpdateFrame(animSpeed, idleAnimStartFrame, idleAnimTotalFrames);
            
            
            break;
        }

        case EnemyState::MOVE_LEFT: {
            GetSprite()->SetSrcY(moveAnimYOffset);
            UpdateFrame(animSpeed, moveAnimStartFrame, moveAnimTotalFrames);

            MoveX(moveSpeed * -1);
	    
            GetSprite()->SetFlip(SDL_FLIP_HORIZONTAL);
            break;
        }

        case EnemyState::MOVE_RIGHT: {
            GetSprite()->SetSrcY(moveAnimYOffset);
            UpdateFrame(animSpeed, moveAnimStartFrame, moveAnimTotalFrames);

            MoveX(moveSpeed);
	   
            GetSprite()->SetFlip(SDL_FLIP_NONE);
            break;
        }

    }

    //adhere to screen bounds
    if(this->GetSprite()->GetX() < 0){this->GetSprite()->SetX(0);}

    if(this->GetSprite()->GetX() > game4->getBgWidth()- this->GetSprite()->GetW()){//Background change
        this->GetSprite()->SetX(game4->getBgWidth() - this->GetSprite()->GetW());//Background change
    }

    return;
}

void Enemy::DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith){ 
    switch(objCollidedWith->GetType()){
        case ObjType::Player: {
            //cast to Player
            auto player = std::dynamic_pointer_cast<Player>(objCollidedWith);
            player->ChangeHealth(damagePerHit * -1);
	    player->SetDamaged(1);
            break;
        }

        default: {
            break;
        }
    }
}

void Enemy::SetIdle(){
    SetEnemyState(EnemyState::IDLE);
}

std::string Enemy::PrintObjType(){
    return "Enemy";
}

int Enemy::UpdateFrame(int frame_duration, int start_frame, int totalFrameCount){
    int frame = (SDL_GetTicks() / frame_duration) % totalFrameCount;
    GetSprite()->SetSrcX(frame * GetSprite()->GetSrcW());

    return frame;
}
