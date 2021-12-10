#include "Text.h"
#include <iostream>

Text::Text(){
    font = NULL;
    renderer = NULL;
    texture = NULL;
    textBox.x = 0;
    textBox.y = 0;
    textBox.w = 0;
    textBox.h = 0;
    textColor.r = 0;
    textColor.g = 0;
    textColor.b = 0;
    currText = "";
}

void Text::Init(SDL_Renderer *ren, const char *ttf_file, int fontSize, int x, int y, SDL_Color color){
    renderer = ren;
    font = TTF_OpenFont(ttf_file, fontSize);
    if(!font) std::cout << "Font did not load" << std::endl;

    textBox.x = x;
    textBox.y = y;

    textColor = color;
}

TTF_Font* Text::GetFont(){ return font; }
int Text::GetW(){ return textBox.w; }
int Text::GetH(){ return textBox.h; }
int Text::GetX(){ return textBox.x; }
int Text::GetY(){ return textBox.y; }

void Text::SetText(const char *text){
    if(strcmp(currText.c_str(), text) != 0){
        currText = std::string(text);
        SDL_Surface *surface;
        if(!(surface = TTF_RenderText_Solid(font, text, textColor))){
            std::cout << "Font Error" << std::endl;
        } else{
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            int w, h;
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);

            textBox.w = w;
            textBox.h = h;
        }
    }
}

void Text::SetX(int x){ textBox.x = x;}
void Text::SetY(int y){ textBox.y = y;}

void Text::Render(){
    SDL_RenderCopy(renderer, texture, NULL, &textBox);
}



Text::~Text(){
    //if(font) TTF_CloseFont(font);
}