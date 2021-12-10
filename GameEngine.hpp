/**
	Patrick Goodwin
	CPSC 4160
	Dr. Victor Zordan
	Assignment 2
**/

#ifndef GAME_ENGINE
#define GAME_ENGINE

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_timer.h>
#include <iostream>
#include <string>
#include "GameObject.hpp"

// Define 255 max for some SDL function calls
#define MAX 255

struct SDLInfo
{
	SDL_Window* my_window;
	SDL_Renderer* my_renderer;
	SDL_Event input;
	SDL_Surface* my_image;
	SDL_Texture* my_texture;
	SDLInfo(): my_window(NULL), my_renderer(NULL) {}
};

class GameEngine
{
	private:
	// Screen dimension
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDLInfo sdlInfo;

	public:
	// Create the window
	GameEngine(const char*);
	// Close program
	~GameEngine();
	// Create the image
	void create_image(const char*);
	// Display window
	void display_window();
	// Create Object
	GameObject makeObject(int, int, int, int);
};


#endif