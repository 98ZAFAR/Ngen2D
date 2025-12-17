#pragma once
#include <SDL.h>

class SDLApp{
    public:
        bool Init();
        void Shutdown();
        bool IsRunning() const;
        void HandleEvents();
        void render();
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        bool isRunning = true;
};