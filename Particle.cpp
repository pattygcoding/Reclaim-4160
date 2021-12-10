/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Fall 2020
*/

#include "Particle.h"

#include <string>
#include <iostream>

Particle::Particle(){};

void Particle::Init(SDL_Renderer *ren, const char *file, int startX, int startY, int startW, int startH){
    renderer = ren;

    SDL_Surface *surface = IMG_Load(file);
    graphic = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    graphic_rect.x = startX;
    graphic_rect.y = startY;
    graphic_rect.w = startW;
    graphic_rect.h = startH;

}

void Particle::Update(){
    switch(particleType){
        case ParticleType::RISE_FADEOUT:{
            if(lifetime > 0){
                graphic_rect.x += XVel;
                graphic_rect.y += YVel;
                lifetime -= 1; //decrease duration of particle per game loop
            }
            break;
        }
    }
    return;
}

void Particle::Render(){
    if(lifetime > 0){
        SDL_RenderCopy(renderer, graphic, NULL, &graphic_rect);
    }
}

void Particle::SetXVel(double v){ XVel = v;} 
void Particle::SetYVel(double v){ YVel = v; }

/*
    Lifetime is the number of game loops the particle will survive for.
    Particle will not be rendered when lifetime is 0.

    params:
        t = number of game loops the particle should survive for. negative input resets to 0
*/
void Particle::SetLifetime(int t){ 
    if(t >= 0) lifetime = t; 
    else lifetime = 0;
}

void Particle::SetParticleType(ParticleType type){ particleType = type; }

double Particle::GetXVel(){ return XVel; }
double Particle::GetYVel(){ return YVel; }
int Particle::GetLifetime(){ return lifetime; }
ParticleType Particle::GetParticleType(){ return particleType; }