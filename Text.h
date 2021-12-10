#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Text{
    private:
        SDL_Renderer *renderer;
        SDL_Rect textBox;
        TTF_Font *font;
        SDL_Color textColor;
        SDL_Texture *texture;
        std::string currText;
    public:
        Text();
        ~Text();
        void Init(SDL_Renderer *ren, const char *ttf_file, int fontSize, int x, int y, SDL_Color color);
        void Render();

        TTF_Font *GetFont();
        int GetX();
        int GetY();
        int GetW();
        int GetH();

        void SetText(const char *text);
        void SetX(int x);
        void SetY(int y);
};

#endif