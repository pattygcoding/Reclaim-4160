/**
	Patrick Goodwin
	CPSC 4160
	Dr. Victor Zordan
	Assignment 2
**/

#include "GameObject.hpp"
#include <iostream>
#include <string>


GameObject::GameObject(SDLObjectInfo sdlObjectInfo, int x, int y, int w, int h)
{
	SDL_RenderClear(sdlObjectInfo.my_renderer);
	SDL_RenderCopy(sdlObjectInfo.my_renderer, sdlObjectInfo.my_texture, NULL, NULL);
	create_rect(sdlObjectInfo, x, y, w, h);
	SDL_RenderPresent(sdlObjectInfo.my_renderer);
}

void GameObject::create_rect(SDLObjectInfo sdlObjectInfo, int x, int y, int w, int h)
{
	my_rect.x = x;
	my_rect.y = y;
	my_rect.w = w;
	my_rect.h = h;
	SDL_SetRenderDrawColor(sdlObjectInfo.my_renderer, MAX, MAX, MAX, MAX);
 	SDL_RenderFillRect(sdlObjectInfo.my_renderer, &my_rect);
	SDL_SetRenderDrawColor(sdlObjectInfo.my_renderer, 0, 0, 0, MAX);
}