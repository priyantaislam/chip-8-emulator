#include <stdio.h>
#include "SDL2/SDL.h"

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(
        "Chip8 Window", 
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640, 320, SDL_WINDOW_SHOWN 
    );
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    while(1){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                goto out;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        
    }
out:
    SDL_DestroyWindow(window);
    return 0;
}