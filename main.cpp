/*
    Patrick Goodwin
    Dr. Zordan CPSC 4160
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <iostream>
#include <string>

#include "GameEngine.h"

#define SCREEN_WIDTH 680
#define SCREEN_HEIGHT 480

const int fps = 60;
const int frame_duration = 1 + 1000/fps;

int main() 
{
    GameEngine *game = GameEngine::get_instance();
    game->init(SCREEN_WIDTH, SCREEN_HEIGHT);

    int this_start_time = 0;
    
    #ifdef FPS_COUNTER_IN_TERMINAL
    //variables for fps counter
    int last_count_start_time = 0;
    int current_fps = 0;
    int fps_counter = 0;
    #endif
    
    //variables for fps limiter
    int this_duration = 0;

    //game loop
    while (game->GetRunningState()) {
        this_start_time = SDL_GetTicks();
        
        game->HandleEvents();
        game->Update();
        game->Render();

        #ifdef FPS_COUNTER_IN_TERMINAL
        //FPS counter
        fps_counter++;
        if(this_start_time >= (last_count_start_time + 1000)){
            last_count_start_time = this_start_time;
            current_fps = fps_counter;
            fps_counter = 0;
        }
        std::cout << "\r" << current_fps << std::flush; //FPS counter updates on same line instead of stacking in terminal
        #endif

        //FPS limiter
        this_duration = SDL_GetTicks() - this_start_time;
        if(this_duration < frame_duration) {
            SDL_Delay(frame_duration - this_duration);
        }
    }
   
    game->Quit();

    return 0;
}
