// CS 4160 - starter code written by Victor Zordan

/**
	Patrick Goodwin
	CPSC 4160
	Dr. Victor Zordan
	Assignment 1
**/

#include <iostream>
#include <string>
#include "GameEngine.hpp"

int main() 
{
	GameEngine gameEngine("my_game");
    gameEngine.create_image("pi.bmp");
	gameEngine.display_window();
    return 0; 
}
