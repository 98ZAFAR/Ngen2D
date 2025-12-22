#include "SDLApp.h"
#include "../engine/core/Config.h"
#include <iostream>
#include "../engine/shapes/AABBShape.h"
#include "../engine/shapes/CircleShape.h"

bool SDLApp::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return false;

    window = SDL_CreateWindow(Config::WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    return window && renderer;
}

void SDLApp::Shutdown()
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

bool SDLApp::IsRunning() const
{
    return isRunning;
}

void SDLApp::HandleEvents(PhysicsWorld &world)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            float mouseX = static_cast<float>(event.button.x);
            float mouseY = static_cast<float>(event.button.y);

            std::cout << "Mouse Clicked at: (" << mouseX << ", " << mouseY << ")\n";
            RigidBody *entity = new RigidBody(1.0f);
            entity->position = Vector2(mouseX, mouseY);
            entity->size = Vector2(30.0f, 30.0f);

            // entity->collider = new Collider(new AABBShape(entity->size/2));
            entity->collider = new Collider(new CircleShape(entity->size.x / 2));
            entity->collider->restitution = 0.9f; // Set some bounciness
            entity->collider->staticFriction = 0.2f;
            entity->collider->dynamicFriction = 0.1f;
            entity->velocity = Vector2(400.0f, 0.0f);
            entity->SetInverseInertia(entity->collider->shape->GetType());

            world.AddBody(entity);
        }
    }
}

void SDLApp::Clear()
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
}

void SDLApp::DrawRect(RigidBody body, float x, float y, int w, int h, SDL_Color color)
{
    DrawRotatedRect(x, y, w, h, body.orientation * 180.0f / M_PI, color);
}

void SDLApp::DrawRotatedRect(float x, float y, int w, int h, float angle, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    // Convert angle to radians
    float rad = angle * M_PI / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);
    
    // Calculate corners relative to center
    float hw = w / 2.0f;
    float hh = h / 2.0f;
    
    SDL_Point corners[4];
    corners[0] = {static_cast<int>(x + (-hw * cosA - (-hh) * sinA)), 
                  static_cast<int>(y + (-hw * sinA + (-hh) * cosA))};
    corners[1] = {static_cast<int>(x + (hw * cosA - (-hh) * sinA)), 
                  static_cast<int>(y + (hw * sinA + (-hh) * cosA))};
    corners[2] = {static_cast<int>(x + (hw * cosA - hh * sinA)), 
                  static_cast<int>(y + (hw * sinA + hh * cosA))};
    corners[3] = {static_cast<int>(x + (-hw * cosA - hh * sinA)), 
                  static_cast<int>(y + (-hw * sinA + hh * cosA))};
    
    // Draw the four edges
    SDL_RenderDrawLine(renderer, corners[0].x, corners[0].y, corners[1].x, corners[1].y);
    SDL_RenderDrawLine(renderer, corners[1].x, corners[1].y, corners[2].x, corners[2].y);
    SDL_RenderDrawLine(renderer, corners[2].x, corners[2].y, corners[3].x, corners[3].y);
    SDL_RenderDrawLine(renderer, corners[3].x, corners[3].y, corners[0].x, corners[0].y);
    
    // // Draw diagonal for orientation indicator
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderDrawLine(renderer, static_cast<int>(x), static_cast<int>(y), corners[1].x, corners[1].y);
}

void SDLApp::DrawCircle(float xc, float yc, int r, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Midpoint circle algorithm
    int x = 0;
    int y = r;

    int p = 1 - r;
    while (x <= y)
    {
        plotPixel(xc + x, yc + y);
        plotPixel(xc - x, yc + y);
        plotPixel(xc + x, yc - y);
        plotPixel(xc - x, yc - y);

        plotPixel(xc + y, yc + x);
        plotPixel(xc - y, yc + x);
        plotPixel(xc + y, yc - x);
        plotPixel(xc - y, yc - x);

        x++;

        if (p < 0)
            p += 2 * x + 1;
        else
        {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
}

void SDLApp::DrawCircleWithIndicator(float xc, float yc, int r, float angle, SDL_Color color)
{
    // Draw circle outline
    DrawCircle(xc, yc, r, color);
    
    // Draw a line from center to edge to show rotation
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red line
    float endX = xc + r * cos(angle);
    float endY = yc + r * sin(angle);
    SDL_RenderDrawLine(renderer, static_cast<int>(xc), static_cast<int>(yc), 
                       static_cast<int>(endX), static_cast<int>(endY));
}

void SDLApp::Paint(PhysicsWorld &world)
{
    for (int i = 0; i < world.GetBodyCount(); i++)
    {
        RigidBody *body = world.GetBody(i);

        if (!body->collider)
            continue;

        if (body->collider->shape->GetType() == ShapeType::AABB)
            DrawRect(*body, body->position.x, body->position.y, static_cast<int>(body->size.x), static_cast<int>(body->size.y), {255, 255, 255, 255});
        else if (body->collider->shape->GetType() == ShapeType::Circle)
        {
            auto *circle = static_cast<CircleShape *>(body->collider->shape);
            DrawCircleWithIndicator(body->position.x, body->position.y, static_cast<int>(circle->radius), body->orientation, {255, 255, 255, 255});
        }
    }

    SDL_RenderPresent(renderer);
}