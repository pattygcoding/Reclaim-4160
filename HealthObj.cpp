/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Fall 2020
*/

#include "HealthObj.h"
#include "Player.h"

#include <string>
#include <SDL2/SDL_image.h>
#include <iostream>



HealthObj::HealthObj(){
    objType = ObjType::Health;
    particleEmitter = std::make_unique<ParticleEmitter>();
    objState = HealthObjState::NOT_COLLECTED;
    //Load sound effects
    if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 256) < 0 ){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    healthSound = Mix_LoadWAV("sounds/Health_Get.wav");
    if(healthSound == NULL) { printf("Unable to load WAV file: %s\n", Mix_GetError()); }
};

HealthObj::~HealthObj(){
    Mix_FreeChunk( healthSound );
    Mix_Quit();
}


HealthObjState HealthObj::GetObjState(){ return objState; }
HealthType HealthObj::GetHealthType(){ return healthType; }
std::shared_ptr<ParticleEmitter> HealthObj::GetParticleEmitter(){ return particleEmitter;}

void HealthObj::SetObjState(HealthObjState state){ objState = state; }
void HealthObj::SetHealthType(HealthType type){
    healthType = type;

    int spriteFrameWidth, spriteFrameHeight;
    double scale = 0.1;
    if(healthType == HealthType::SANITIZER){
        spriteFrameWidth = 247*scale;
        spriteFrameHeight = 247*scale;
        particleEmitter->Init(GetRenderer(), "img/healthsparkle.png", GetSprite()->GetX() + GetSprite()->GetW()/2 - spriteFrameWidth/2,
                                                GetSprite()->GetY() + GetSprite()->GetH() - spriteFrameHeight,
                                                GetSprite()->GetW()/2, GetSprite()->GetH() - spriteFrameHeight,
                                                spriteFrameWidth, spriteFrameHeight, ParticleType::RISE_FADEOUT);
    }
    else if(healthType == HealthType::WIPES){

    }
}
void HealthObj::ResetSprite(){
    ChangeAlpha(255);
    SDL_SetTextureAlphaMod(GetSprite()->GetTexture(),GetAlpha());
}

//overrides
void HealthObj::Update(){
    if(objState == HealthObjState::COLLECTED){
        SetBoxCollider(0,0,0,0); //remove collider
        if(healthType == HealthType::SANITIZER){
            particleEmitter->Update();
        }
        else if(healthType == HealthType::WIPES){

        }
    }
}

void HealthObj::Render(){
    if(objState == HealthObjState::COLLECTED){
        if(healthType == HealthType::SANITIZER){
            ChangeAlpha(-1 * (255/particleEmitter->GetDuration() + 0.5)); //0.5 is added so that the truncation of 255/pe->GetDuration is rounded up
            SDL_SetTextureAlphaMod(GetSprite()->GetTexture(),GetAlpha());
            particleEmitter->Render();
        }
        else if(healthType == HealthType::WIPES){

        }
    }

    SDL_Rect* temp = GetSprite()->GetScreenRect();//Background change
    temp->x = temp->x- GameObject::GetScreenRecX();//Background change
    SDL_RenderCopy(GetRenderer(), GetSprite()->GetTexture(), GetSprite()->GetSrcRect(), temp);//Background change
    temp->x = temp->x+ GameObject::GetScreenRecX();//Background change
}

void HealthObj::DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith){
    switch(objCollidedWith->GetType()){
        case ObjType::Player: {
            if(objState != HealthObjState::COLLECTED){
                SetObjState(HealthObjState::COLLECTED);
                Mix_PlayChannel( -1, healthSound, 0 );
                break;
            }
        }

        default: {
            break;
        }
    }
}

std::string HealthObj::PrintObjType(){ return "HealthObj"; }
