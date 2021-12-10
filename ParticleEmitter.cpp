/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Fall 2020
*/

#include "ParticleEmitter.h"

#include <string>
#include <iostream>
#include <stdlib.h>

ParticleEmitter::ParticleEmitter(){
    duration = 0;
};

const int MAX_PARTICLES_RISEFADEOUT = 8;

int randRange(int lower, int upper){
    return (lower + rand() % (upper - lower + 1));
}

/*
    Call this function before any other ParticleEmitter functions (other than constructor)

    Params:
    ren = an SDL_Renderer
    file = the pathname for the graphic used for each individual particle -- include quotation marks e.g. "./sparkle.png"
    startX = the X coordinate of the game window that this effect will start on
    startY = the Y coordinate of the game window that this effect will start on
    maxOffsetX = maximum number of pixels to offset particle from startX
    maxOffsetY = maximum number of pixels to offset particle from startY
    particleW = the width you want your particle to be at when rendered
    particleH = the height you want your particle to be at when rendered
    scale = the scaling of the rendered particle image. 1 = no scaling; <1 = make smaller; >1 = make bigger
    type = the effect you want to use. valid types are defined in Particle.h
*/
void ParticleEmitter::Init(SDL_Renderer *ren, const char *file, int startX, int startY, int maxOffsetX, int maxOffsetY, int particleW, int particleH, ParticleType type){
    particleType = type;
    switch(particleType){
        case ParticleType::RISE_FADEOUT: {
            for (int i = 0; i < MAX_PARTICLES_RISEFADEOUT; i++){
                auto p = std::make_shared<Particle>();

                //particle starting coordinates
                int p_rectX = startX + randRange(maxOffsetX * -1, maxOffsetX);
                int p_rectY = startY + randRange(0, maxOffsetY);

                p->Init(ren, file, p_rectX, p_rectY, particleW, particleH);
            
                p->SetXVel(0);
                p->SetYVel(-0.5 - (randRange(0, 25)/10.0));

                p->SetLifetime(20 + randRange(0, 15));
                p->SetParticleType(particleType);

                particles.push_back(p);
            }
            
            break;
        }
    }

    //finds the max particle lifetime out of all particles of an effect
    //the max particle lifetime = the duration of the effect
    for(auto p : particles){
        if(p->GetLifetime() > duration) duration = p->GetLifetime();
    }
}

void ParticleEmitter::Update(){
    switch(particleType){
        case ParticleType::RISE_FADEOUT: {
            for(auto p : particles){
               p->Update();
            }

            break;
        }
    }
}

void ParticleEmitter::Render(){
    for(auto p : particles){
        p->Render();
    }
}

void ParticleEmitter::Quit(){

}

//duration is the lifetime of the effect
int ParticleEmitter::GetDuration(){ return duration; }