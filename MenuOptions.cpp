#include "MenuOptions.h"
#include <cmath>
#include <iostream>

MenuOptions::MenuOptions(){
    optionsTotal = 0;
    currOptionIndex = 0;
}

void MenuOptions::Init(SDL_Renderer *ren, 
                       const char *selector_img, 
                       int selector_w, 
                       int selector_h, 
                       double img_scale,
                       std::vector<std::shared_ptr<Text>> vector){
    optionsText = vector;
    optionsTotal = vector.size();

    renderer = ren;
    selector = new Sprite(renderer, selector_img);
    selector->SetSrcRect(0, 0, selector_w, selector_h);
    //setting screen rect
    int scaled_w = selector_w * img_scale;
    int scaled_h = selector_h * img_scale;
    selector->SetW(scaled_w);
    selector->SetH(scaled_h);
    selector->SetX(optionsText.at(0)->GetX() - scaled_w - 5);
    selector->SetY(optionsText.at(0)->GetY());
}

void MenuOptions::SelectNextOption(){
    currOptionIndex = (currOptionIndex + 1) % optionsTotal;
    selector->SetX(optionsText.at(currOptionIndex)->GetX() - selector->GetW() - 5);
    selector->SetY(optionsText.at(currOptionIndex)->GetY());
}

void MenuOptions::SelectPrevOption(){
    currOptionIndex = currOptionIndex - 1;
    if(currOptionIndex < 0) currOptionIndex = optionsTotal - 1;
    selector->SetX(optionsText.at(currOptionIndex)->GetX() - selector->GetW() - 5);
    selector->SetY(optionsText.at(currOptionIndex)->GetY());
}

int MenuOptions::GetCurrentOption(){
    return currOptionIndex;
}

void MenuOptions::Render(){
    for(auto text : optionsText){
        text->Render();
    }
    SDL_RenderCopy(renderer, selector->GetTexture(), selector->GetSrcRect(), selector->GetScreenRect());
}
