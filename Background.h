#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Background{
  private:
    SDL_Renderer *renderer;
    SDL_Texture* texture;
    SDL_Rect BG_rect;
    SDL_Rect* screen_rect;
    const char *bgfile;

  public:
    Background();
    ~Background();

    Background(SDL_Renderer *render, const char *file,int x, int y, int w, int h, SDL_Rect* camera);

    int getMapWidth();
    int getMapHeight();
    int getScreenx();

    void render();
    void moveScreenX(SDL_Rect x);

};
#endif
