/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Assignment 3
    October 23 2020
*/

#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>

class Sprite{
    private:
        SDL_Texture* texture;
        SDL_Rect src_rect; //defines the part of the texture that will be rendered
        SDL_Rect screen_rect; //determines how the image defined in src_rect will be rendered onto the screen
        SDL_RendererFlip flip;
    public:
        Sprite();
        ~Sprite();

        Sprite(SDL_Renderer *renderer, const char *file);
        int UpdateFrame(int frame_duration, int start_frame, int totalFrameCount);

        void SetSrcRect(int x, int y, int w, int h);
        void SetScreenRect(int x, int y, int w, int h);
        void SetTexture(SDL_Renderer *renderer, const char *file);
        void SetFlip(SDL_RendererFlip f);

        void AddX(int x);//background change
        void SetX(int x);
        void SetY(int y);
        void SetW(int w);
        void SetH(int h);

        void SetSrcX(int x);
        void SetSrcY(int y);
        void SetSrcW(int w);
        void SetSrcH(int h);

        SDL_Rect* GetScreenRect();
        SDL_Rect* GetSrcRect();
        SDL_Texture* GetTexture();
        SDL_RendererFlip GetFlip();

        int GetX();
        int GetY();
        int GetW();
        int GetH();

        int GetSrcX();
        int GetSrcY();
        int GetSrcW();
        int GetSrcH();
};


#endif
