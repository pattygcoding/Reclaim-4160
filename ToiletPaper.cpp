#include "ToiletPaper.h"
#include "Player.h"

#include <string>
#include <SDL2/SDL_image.h>
#include <iostream>



ToiletPaper::ToiletPaper(){
    objType = ObjType::TP;
    objState = TPObjState::NOT_COLLECTED;
    fadeDuration = 10;
};

ToiletPaper::~ToiletPaper(){
    
}


TPObjState ToiletPaper::GetObjState(){ return objState; }

void ToiletPaper::SetObjState(TPObjState state){ objState = state; }

void ToiletPaper::ResetSprite(){
  ChangeAlpha(255);
  SDL_SetTextureAlphaMod(GetSprite()->GetTexture(),GetAlpha());
}

//overrides
void ToiletPaper::Update(){
    if(objState == TPObjState::COLLECTED){
        SetBoxCollider(0,0,0,0); //remove collider
    }
}

void ToiletPaper::Render(){
    if(objState == TPObjState::COLLECTED){
        ChangeAlpha(-1 * (255/fadeDuration + 0.5)); //0.5 is added so that the truncation of 255/pe->GetDuration is rounded up
        SDL_SetTextureAlphaMod(GetSprite()->GetTexture(),GetAlpha());
    }

  SDL_Rect* temp = GetSprite()->GetScreenRect();//Background change
  temp->x = temp->x- GameObject::GetScreenRecX();//Background change
  SDL_RenderCopy(GetRenderer(), GetSprite()->GetTexture(), GetSprite()->GetSrcRect(), temp);//Background change
  temp->x = temp->x+ GameObject::GetScreenRecX();//Background change
}

void ToiletPaper::DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith){
    switch(objCollidedWith->GetType()){
        case ObjType::Player: {
            auto player = std::dynamic_pointer_cast<Player>(objCollidedWith);
            if(objState != TPObjState::COLLECTED){
                SetObjState(TPObjState::COLLECTED);
                player->GotTP(true);
                break;
            }
        }

        default: {
            break;
        }
    }
}

std::string ToiletPaper::PrintObjType(){ return "ToiletPaper"; }
