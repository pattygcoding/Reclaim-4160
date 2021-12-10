#include "Sprite.h"

#include <string>
#include <SDL2/SDL_image.h>
#include <iostream>

Sprite::Sprite(){ }

/*
    Sprite constructor should be called from GameObject only
    Creates a texture with the desired image file

    Parameters (same in GameObject init):
    ren = renderer
    file = path of image file used for the sprite (example: "./spritesheet.png")
*/
Sprite::Sprite(SDL_Renderer *renderer, const char *file){
    SDL_Surface *surface = IMG_Load(file);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    flip = SDL_FLIP_NONE;
}

void Sprite::SetSrcRect(int x, int y, int w, int h){
    src_rect.x = x;
    src_rect.y = y;
    src_rect.w = w;
    src_rect.h = h;
}

void Sprite::SetScreenRect(int x, int y, int w, int h){
    screen_rect.x = x;
    screen_rect.y = y;
    screen_rect.w = w;
    screen_rect.h = h;
}

void Sprite::SetTexture(SDL_Renderer *renderer, const char *file){
    SDL_Surface *surface = IMG_Load(file);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

/*
    Update the sprite to show the next animation frame

    Parameters:
    frame_duration = how long one animation frame remains on screen before going onto the next frame. timed by milliseconda
    start_frame = the index of the animation frame that you want to start on, starting from 0.
    totalFrameCount = the number of animation frames you want to cycle through

    frame_duration and start_frame cannot be negative
    totalFrameCount cannot be <= 0

*/
int Sprite::UpdateFrame(int frame_duration, int start_frame, int totalFrameCount){
    src_rect.x += src_rect.w * start_frame; //set the frame the animation starts on

    int frame_time = (int)(SDL_GetTicks()/frame_duration) % totalFrameCount;
    static int last_frame_time = 0;

    if(last_frame_time != frame_time){
        if(frame_time < 1) src_rect.x = 0;
        else src_rect.x += src_rect.w;
    }

    last_frame_time = frame_time;

    return frame_time;
}
//I needed this to move the spite for the menu items without losing thier original position
void Sprite::AddX(int x){screen_rect.x = screen_rect.x+x;};//background change
//set the dimensions of the rectangle that will be rendered onto the screen
void Sprite::SetX(int x){screen_rect.x = x;}
void Sprite::SetY(int y){screen_rect.y = y;}
void Sprite::SetW(int w){screen_rect.w = w;}
void Sprite::SetH(int h){screen_rect.h = h;}

//set the dimensions of the rectangle that will select the area of the texture to render
void Sprite::SetSrcX(int x){src_rect.x = x;}
void Sprite::SetSrcY(int y){src_rect.y = y;}
void Sprite::SetSrcW(int w){src_rect.w = w;}
void Sprite::SetSrcH(int h){src_rect.h = h;}

void Sprite::SetFlip(SDL_RendererFlip f){flip=f;}

SDL_Rect* Sprite::GetSrcRect(){return &src_rect;}
SDL_Rect* Sprite::GetScreenRect(){return &screen_rect;}
SDL_Texture* Sprite::GetTexture(){return texture;}
SDL_RendererFlip Sprite::GetFlip(){return flip;}

int Sprite::GetX(){return screen_rect.x;}
int Sprite::GetY(){return screen_rect.y;}
int Sprite::GetW(){return screen_rect.w;}
int Sprite::GetH(){return screen_rect.h;}

int Sprite::GetSrcX(){return src_rect.x;}
int Sprite::GetSrcY(){return src_rect.y;}
int Sprite::GetSrcW(){return src_rect.w;}
int Sprite::GetSrcH(){return src_rect.h;}

Sprite::~Sprite(){
    SDL_DestroyTexture(texture);
}
