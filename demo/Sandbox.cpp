#include "Sandbox.h"
#include "../engine/core/Time.h"
#include "../engine/core/Config.h"
#include "../engine/shapes/AABBShape.h"
#include "../engine/shapes/CircleShape.h"
#include "../engine/collision/Collider.h"
#include "../engine/forces/GravityForce.h"

#include <chrono>

// Initialize the sandbox with a box, ground and walls
Sandbox::Sandbox() {
    //Gravity Initialization
    auto gravity = std::make_unique<GravityForce>(Vector2(0.0f, Config::GRAVITY));
    world.AddForceGenerator(std::move(gravity));

    // Initialize timing
    lastTime = std::chrono::high_resolution_clock::now();

    // Box Initialization
    auto boxBody = std::make_unique<RigidBody>(1.0f);
    boxBody->position = {200.0f, 100.0f};
    boxBody->orientation = 1.0f;
    boxBody->velocity = {0.0f, 0.0f};
    boxBody->size = {50.0f, 80.0f};
    boxBody->collider = std::make_unique<Collider>(std::make_unique<AABBShape>(Vector2(boxBody->size/2)));
    boxBody->collider->restitution = 0.6f;
    boxBody->collider->staticFriction = 0.5f;
    boxBody->collider->dynamicFriction = 0.4f;
    boxBody->SetInverseInertia(boxBody->collider->shape->GetType());
    box = boxBody.get();
    world.AddBody(std::move(boxBody));

    //Ground Initialization
    auto groundBody = std::make_unique<RigidBody>(0.0f);
    groundBody->position = {600.0f, 775.0f};
    groundBody->size = {1200.0f, 50.0f};
    groundBody->collider = std::make_unique<Collider>(std::make_unique<AABBShape>(Vector2(groundBody->size/2)));
    groundBody->collider->restitution = 0.6f; 
    groundBody->collider->staticFriction = 0.3f;
    groundBody->collider->dynamicFriction = 0.2f;
    ground = groundBody.get();
    world.AddBody(std::move(groundBody));

    //left_Wall Initialization
    auto left_wall = std::make_unique<RigidBody>(0.0f);
    left_wall->size = Vector2(50.0f, 750.0f);
    left_wall->position = Vector2(25.0f, 375.0f);
    left_wall->collider = std::make_unique<Collider>(std::make_unique<AABBShape>(Vector2(left_wall->size/2)));
    left_wall->collider->restitution = 0.6f; 
    left_wall->collider->staticFriction = 0.3f;
    left_wall->collider->dynamicFriction = 0.2f;
    world.AddBody(std::move(left_wall));

    //Right_Wall Initialization
    auto right_wall = std::make_unique<RigidBody>(0.0f);
    right_wall->size = Vector2(50.0f, 750.0f);
    right_wall->position = Vector2(1175.0f, 375.0f);
    right_wall->collider = std::make_unique<Collider>(std::make_unique<AABBShape>(Vector2(right_wall->size/2)));
    right_wall->collider->restitution = 0.6f; 
    right_wall->collider->staticFriction = 0.3f;
    right_wall->collider->dynamicFriction = 0.2f;
    world.AddBody(std::move(right_wall));
}

// Update the sandbox state
void Sandbox::Update(){
    // Calculate delta time (frame time)
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    
    float frameTime = deltaTime.count();
    
    if(frameTime > 0.25f)
        frameTime = 0.25f;
    
    accumulator += frameTime;
    
    while(accumulator >= Time::FixedDeltaTime){
        world.Step(Time::FixedDeltaTime);
        accumulator -= Time::FixedDeltaTime;
    }
}