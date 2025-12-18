#pragma once
#include <SDL.h>

class SDLApp{
    public:
        bool Init();
        void Shutdown();
        bool IsRunning() const;
        void HandleEvents();
        void Render();
        void DrawRect(float x, float y, int w, int h, SDL_Color color);
        void Clear();
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        bool isRunning = true;
};