#include "PushableObj.h"
#include "GameEngine.h"

#include <string>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Player.h"

PushableObj::PushableObj(){
    objType = ObjType::Pushable;
    objState = PushableObjState::IDLE;
    LastMovementState = PushableObjState::IDLE;
    pushForce = 0;
    velocity = 0;
    PushedThisTick = 0;
};

GameEngine* game2 = GameEngine::get_instance();

PushableObjState PushableObj::GetObjState(){return objState;}
PushableObjState PushableObj::GetLastMovementState(){return LastMovementState;}
double PushableObj::GetPushForce(){return pushForce;}
double PushableObj::GetVelocity(){return velocity;}
int PushableObj::GetPushedtick(){return PushedThisTick;}
int PushableObj::GetSpeed(){
    if(velocity > 0) return velocity;
    else return -velocity;
}

/*
    state = one of the valid states that Player can have. Valid states (defined in header file) are IDLE, PUSHED_FROM_LEFT, PUSHED_FROM_RIGHT
*/
void PushableObj::SetPushedTick(int pushed){PushedThisTick=pushed;}
void PushableObj::SetObjState(PushableObjState state){
    objState = state;
        if(velocity==0){
        LastMovementState=state;
    }
    else if(state!=PushableObjState::IDLE){
        LastMovementState=state;
    }
}
void PushableObj::SetVelocity(int speed){velocity = speed;}
void PushableObj::SetPushForce(int force){pushForce = force;}

//Updates object's position based on object's state
void PushableObj::Update(){
    //dampen = amount of slow-down when the object goes from moving to idle
    //closer to 0 means more abrupt stop
    double dampen = 0.15;

    switch(objState){
        case PushableObjState::IDLE: {
            //Dampening effect
            if(velocity < 0){ velocity += dampen; }
            else if(velocity > 0){ velocity -= dampen; }
            else velocity = 0;

            MoveX(velocity);
            if(GetPushedtick()>0){
              SetPushedTick(GetPushedtick()-1);
            }
            break;
        }

        case PushableObjState::PUSHED_FROM_LEFT: {
            velocity = pushForce * -1;
            MoveX(velocity);
            SetIdle();
            break;
        }

         case PushableObjState::PUSHED_FROM_RIGHT: {
            velocity = pushForce;
            MoveX(velocity);
            SetIdle();
            break;
        }
    }

    //prevent object from crossing left bound (left edge of screen)
    if(this->GetSprite()->GetX() < 0){this->GetSprite()->SetX(0);}

    //prevent object from crossing right bound (right edge of screen)
    if(this->GetSprite()->GetX() > game2->getBgWidth() - this->GetSprite()->GetW()){//Background change
        this->GetSprite()->SetX(game2->getBgWidth() - this->GetSprite()->GetW());//Background change
    }

    //link box collider position to object position
    this->SetBoxColliderPos(this->GetSprite()->GetX(), this->GetSprite()->GetY());
    return;
}

void PushableObj::DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith){
    switch(objCollidedWith->GetType()){
        case ObjType::Pushable: {
          //cast to Pushable
          auto pushob = std::dynamic_pointer_cast<PushableObj>(objCollidedWith);
          if(GetSpeed() < pushob->GetSpeed()){
            if(GetPushedtick()<=0){
                if(pushob->GetObjState() == PushableObjState::PUSHED_FROM_RIGHT){
                    SetObjState(PushableObjState::PUSHED_FROM_RIGHT);
                    pushob->SetObjState(PushableObjState::PUSHED_FROM_LEFT);
                    SetPushedTick(1);
                    pushob->SetPushedTick(1);
                }
                else if(pushob->GetLastMovementState() == PushableObjState::PUSHED_FROM_RIGHT){
                    SetObjState(PushableObjState::PUSHED_FROM_RIGHT);
                    pushob->SetObjState(PushableObjState::PUSHED_FROM_LEFT);
                    SetPushedTick(1);
                    pushob->SetPushedTick(1);
                }
                else if(pushob->GetObjState() == PushableObjState::PUSHED_FROM_LEFT){
                    SetObjState(PushableObjState::PUSHED_FROM_LEFT);
                    pushob->SetObjState(PushableObjState::PUSHED_FROM_RIGHT);
                    SetPushedTick(1);
                    pushob->SetPushedTick(1);
                }
                else if(pushob->GetLastMovementState() == PushableObjState::PUSHED_FROM_LEFT){
                    SetObjState(PushableObjState::PUSHED_FROM_LEFT);
                    pushob->SetObjState(PushableObjState::PUSHED_FROM_RIGHT);
                    SetPushedTick(1);
                    pushob->SetPushedTick(1);
                }
            }
        }
        break;
      }
        case ObjType::Player: {
            //cast to Player
            auto player = std::dynamic_pointer_cast<Player>(objCollidedWith);
            if(GetPushedtick()==0){
              if(player->GetPlayerState() == PlayerState::MOVE_LEFT){
                    SetObjState(PushableObjState::PUSHED_FROM_LEFT);
                    LastMovementState = PushableObjState::PUSHED_FROM_LEFT;
              }
              else if(player->GetPlayerState() == PlayerState::MOVE_RIGHT){
                    SetObjState(PushableObjState::PUSHED_FROM_RIGHT);
                    LastMovementState = PushableObjState::PUSHED_FROM_RIGHT;
              }
            }
            break;
        }

        default: {
            break;
        }
    }
}

void PushableObj::SetIdle(){
    SetObjState(PushableObjState::IDLE);
    if(velocity == 0) SetObjState(PushableObjState::IDLE);
}

std::string PushableObj::PrintObjType(){ return "PushableObj"; }
