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
    box.velocity = {0.0f, 0.0f};
    box.size = {50.0f, 50.0f};
    box.collider = new Collider(new AABBShape(Vector2(box.size/2)));
    box.collider->restitution = 0.6f; 
    world.AddBody(&box);

    //Ground Initialization
    ground.position = {600.0f, 775.0f};
    ground.size = {1200.0f, 50.0f};
    ground.collider = new Collider(new AABBShape(Vector2(ground.size/2)));
    ground.collider->restitution = 0.6f; 
    ground.collider->staticFriction = 0.3f;
    ground.collider->dynamicFriction = 0.2f;
    world.AddBody(&ground);

    // Ball Initialization
    ball.position = {400.0f, 50.0f};
    ball.velocity = {0.0f, 0.0f};
    ball.size = {50.0f, 50.0f};
    ball.collider = new Collider(new CircleShape(ball.size.x / 2));
    ball.collider->restitution = 0.9f;
    ball.collider->staticFriction = 0.3f;
    ball.collider->dynamicFriction = 0.2f;
    world.AddBody(&ball);
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