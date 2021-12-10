/**
	Patrick Goodwin
	CPSC 4160
	Dr. Victor Zordan
	Assignment 2
**/

#include "GameEngine.hpp"
#include <iostream>
#include <string>

GameEngine::GameEngine(const char* name)
{
	// Check SDL initialization
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
	// Enable gpu_enhanced textures
    IMG_Init(IMG_INIT_PNG);
    // Create the window
    sdlInfo.my_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    // Create renderer
    sdlInfo.my_renderer = SDL_CreateRenderer(sdlInfo.my_window,-1,0);
}

void GameEngine::create_image(const char* name)
{
	// Create image pointer
    sdlInfo.my_image = SDL_LoadBMP(name);
    // Create texture
    sdlInfo.my_texture = SDL_CreateTextureFromSurface(sdlInfo.my_renderer, sdlInfo.my_image);
}


void GameEngine::display_window()
{
	// Define exit variable for closing image
	bool exit = false;

    while(!exit)
    {
		SDL_WaitEvent(&sdlInfo.input);
		if(sdlInfo.input.type == SDL_QUIT) exit = true;
		else
		{
			int mouse_x = 0;
			int mouse_y = 0;
			while(SDL_PollEvent(&sdlInfo.input))
			{
        		switch(sdlInfo.input.type)
				{
            		case SDL_MOUSEMOTION:

                		mouse_x = sdlInfo.input.motion.x;
						mouse_y = sdlInfo.input.motion.y;
				}
       	 	}
			GameObject gameObject = makeObject(mouse_x, mouse_y, 160, 160);
    	}
    }
}

GameObject GameEngine::makeObject(int x, int ry, int w, int h)
{
	SDLObjectInfo sdlObjectInfo(sdlInfo.my_renderer, sdlInfo.my_texture);
	int y = SCREEN_HEIGHT - ry - h;
	GameObject gameObject(sdlObjectInfo, x, y, w, h);
	return gameObject;
}

GameEngine::~GameEngine()
{
	// Closing process for program
	SDL_DestroyTexture(sdlInfo.my_texture);
	SDL_FreeSurface(sdlInfo.my_image);
    SDL_DestroyRenderer(sdlInfo.my_renderer);
    SDL_DestroyWindow(sdlInfo.my_window);

    IMG_Quit();
    SDL_Quit();
}