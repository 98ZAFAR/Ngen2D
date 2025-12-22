#include "Sandbox.h"
#include "../engine/core/Time.h"
#include "../engine/core/Config.h"
#include "../engine/shapes/AABBShape.h"
#include "../engine/shapes/CircleShape.h"
#include "../engine/collision/Collider.h"
#include "../engine/forces/GravityForce.h"

#include <iostream>
#include <thread>
#include <chrono>

// Initialize the sandbox with a box, ground and ball
Sandbox::Sandbox(): box(1.0f), ground(0.0f), ball(1.0f) {
    //Gravity Initialization
    GravityForce* gravity = new GravityForce(Vector2(0.0f, Config::GRAVITY));
    world.AddForceGenerator(gravity);

    // Initialize timing
    lastTime = std::chrono::high_resolution_clock::now();


    // Box Initialization
    box.position = {200.0f, 100.0f};
    box.orientation = 0.0f;
    box.velocity = {0.0f, 0.0f};
    box.size = {50.0f, 80.0f};
    box.collider = new Collider(new AABBShape(Vector2(box.size/2)));
    box.collider->restitution = 0.6f;
    box.collider->staticFriction = 0.5f;
    box.collider->dynamicFriction = 0.4f;
    box.SetInverseInertia(box.collider->shape->GetType());
    world.AddBody(&box);

    //Ground Initialization
    ground.position = {600.0f, 775.0f};
    ground.size = {1200.0f, 50.0f};
    ground.collider = new Collider(new AABBShape(Vector2(ground.size/2)));
    ground.collider->restitution = 0.6f; 
    ground.collider->staticFriction = 0.3f;
    ground.collider->dynamicFriction = 0.2f;
    world.AddBody(&ground);

    //left_Wall Initialization
    RigidBody* left_wall = new RigidBody(0.0f);
    left_wall->size = Vector2(50.0f, 750.0f);
    left_wall->position = Vector2(25.0f, 375.0f);
    left_wall->collider = new Collider(new AABBShape(Vector2(left_wall->size/2)));
    left_wall->collider->restitution = 0.6f; 
    left_wall->collider->staticFriction = 0.3f;
    left_wall->collider->dynamicFriction = 0.2f;
    world.AddBody(left_wall);

    //Right_Wall Initialization
    RigidBody* right_wall = new RigidBody(0.0f);
    right_wall->size = Vector2(50.0f, 750.0f);
    right_wall->position = Vector2(1175.0f, 375.0f);
    right_wall->collider = new Collider(new AABBShape(Vector2(right_wall->size/2)));
    right_wall->collider->restitution = 0.6f; 
    right_wall->collider->staticFriction = 0.3f;
    right_wall->collider->dynamicFriction = 0.2f;
    world.AddBody(right_wall);
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