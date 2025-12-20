#include "SDLApp.h"
#include "../engine/core/Config.h"
#include <iostream>
#include "../engine/shapes/AABBShape.h"
#include "../engine/shapes/CircleShape.h"

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

void SDLApp::HandleEvents(PhysicsWorld &world){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            isRunning = false;
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN){
            float mouseX = static_cast<float>(event.button.x);
            float mouseY = static_cast<float>(event.button.y);

            std::cout << "Mouse Clicked at: (" << mouseX << ", " << mouseY << ")\n";
            RigidBody* entity = new RigidBody(1.0f);
            entity->position = Vector2(mouseX, mouseY);
            entity->size = Vector2(50.0f, 50.0f);

            entity->collider = new Collider(new CircleShape(entity->size.x / 2));
            entity->collider->restitution = 0.8f; // Set some bounciness
            entity->velocity = Vector2(120.0f, 0.0f);

            world.AddBody(entity);
        }
    }
}


void SDLApp::Clear(){
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
}

void SDLApp::DrawRect(float x, float y, int w, int h, SDL_Color color){
    SDL_Rect rect = {static_cast<int>(x - w / 2), static_cast<int>(y - h / 2), w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void SDLApp::DrawCircle(float x, float y, int radius, SDL_Color color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for(int w = 0; w < radius * 2; w++){
        for(int h = 0; h < radius * 2; h++){
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if((dx*dx + dy*dy) <= (radius * radius)){
                SDL_RenderDrawPoint(renderer, static_cast<int>(x + dx), static_cast<int>(y + dy));
            }
        }
    }
}

void SDLApp::Paint(PhysicsWorld &world){
    // Placeholder for any additional rendering logic specific to the application
    for(int i=0; i<world.GetBodyCount(); i++){
        RigidBody* body = world.GetBody(i);

        if(!body->collider) continue;

        if(body->collider->shape->GetType() == ShapeType::AABB)
            DrawRect(body->position.x, body->position.y, static_cast<int>(body->size.x), static_cast<int>(body->size.y), {255, 255, 255, 255});
        else if(body->collider->shape->GetType() == ShapeType::Circle){
            auto* circle = static_cast<CircleShape*>(body->collider->shape);
            DrawCircle(body->position.x, body->position.y, static_cast<int>(circle->radius), {255, 25, 25, 255});
        }
    }

    SDL_RenderPresent(renderer);
}