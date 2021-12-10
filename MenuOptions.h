#ifndef MENUOPTIONS_H
#define MENUOPTIONS_H

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include "Text.h"
#include "Sprite.h"

class MenuOptions{
    private:
        std::vector<std::shared_ptr<Text>> optionsText;
        SDL_Renderer *renderer;
        int optionsTotal;
        int currOptionIndex;
        Sprite *selector;
    public:
        MenuOptions();
        void Init(SDL_Renderer *ren, const char *selector_img, int selector_w, int selector_h, double img_scale, std::vector<std::shared_ptr<Text>> vector);
        void SelectNextOption();
        void SelectPrevOption();
        int GetCurrentOption();
        void Render();
};

#endif