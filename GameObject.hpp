/**
	Patrick Goodwin
	CPSC 4160
	Dr. Victor Zordan
	Assignment 2
**/

#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_timer.h>
#include <iostream>
#include <string>

// Define 255 max for some SDL function calls
#define MAX 255

struct SDLObjectInfo
{
	SDL_Renderer* my_renderer;
	SDL_Texture* my_texture;
	SDLObjectInfo(SDL_Renderer* renderer, SDL_Texture* texture): my_renderer(renderer), my_texture(texture) {}
};

class GameObject
{
	private:
	// SDL Rectangle definition
	SDL_Rect my_rect;

	public:
	GameObject(SDLObjectInfo, int, int, int, int);
	void create_rect(SDLObjectInfo, int, int, int, int);
};



#endif