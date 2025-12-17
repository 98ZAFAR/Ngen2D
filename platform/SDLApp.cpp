#include "SDLApp.h"
#include "../engine/core/Config.h"

bool SDLApp::Init(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return false;
    
    window = SDL_CreateWindow(Config::WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    return window && renderer;
}

void SDLApp::Shutdown(){
    if(renderer){
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if(window){
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

bool SDLApp::IsRunning() const {
    return isRunning;
}

void SDLApp::HandleEvents(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            isRunning = false;
        }
    }
}

void SDLApp::render(){
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    // Rendering code would go here

    SDL_RenderPresent(renderer);
}