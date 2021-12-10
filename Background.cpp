
#include "Background.h"

Background::Background(){}
Background::~Background(){};
Background::Background(SDL_Renderer *render, const char *file, int x, int y, int w, int h, SDL_Rect* camera){
    renderer = render;
    SDL_Surface *surface = IMG_Load(file);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    BG_rect.x = x;
    BG_rect.y = y;
    BG_rect.w = w;
    BG_rect.h = h;
    screen_rect = camera;
    bgfile = file;
}

int Background::getMapWidth(){return BG_rect.w;}
int Background::getMapHeight(){return BG_rect.h;}
int Background::getScreenx(){return screen_rect->x;}

void Background::moveScreenX(SDL_Rect x){
    screen_rect->x = (x.x+(x.w/2)) - (screen_rect->w/2);

    if(screen_rect->x<0){
        screen_rect->x=0;
    }
    else if(screen_rect->x > BG_rect.w-screen_rect->w){
        screen_rect->x=BG_rect.w-screen_rect->w;
    }
    else if(screen_rect->y<0){
        screen_rect->y=0;
    }
    else if(screen_rect->y > BG_rect.h-screen_rect->h){
        screen_rect->y=BG_rect.h-screen_rect->h;
    }
}

SDL_Rect a{0,0,640,480};
void Background::render(){
    SDL_RenderCopyEx(renderer, texture, screen_rect, &a, 0.0 , NULL, SDL_FLIP_NONE);
}
