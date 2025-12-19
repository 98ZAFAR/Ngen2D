#pragma once
#include <SDL.h>
#include "../engine/physics/PhysicsWorld.h"

class SDLApp{
    public:
        bool Init();
        void Shutdown();
        bool IsRunning() const;
        void HandleEvents(PhysicsWorld &world);
        void DrawRect(float x, float y, int w, int h, SDL_Color color);
        void Paint(PhysicsWorld &world);
        void Clear();
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        bool isRunning = true;
};