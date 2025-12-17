#pragma once
#include <SDL.h>

class SDLApp{
    public:
        bool Init();
        void Run();
        void Shutdown();
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        bool isRunning = true;
};