#pragma once
#include <SDL.h>
#include "../engine/physics/PhysicsWorld.h"

class SDLApp
{
public:
    bool Init();
    void Shutdown();
    bool IsRunning() const;
    void HandleEvents(PhysicsWorld &world);
    void DrawRect(float x, float y, int w, int h, SDL_Color color);
    inline void plotPixel(int x, int y) {SDL_RenderDrawPoint(renderer, x, y);}

    void DrawCircle(float x, float y, int radius, SDL_Color color);
    void Paint(PhysicsWorld &world);
    void Clear();

private:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool isRunning = true;
};