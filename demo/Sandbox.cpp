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

// Initialize the sandbox with a box and ground
Sandbox::Sandbox(): box(1.0f), ground(0.0f), ball(1.0f) {
    //Gravity Initialization
    GravityForce* gravity = new GravityForce(Vector2(0.0f, Config::GRAVITY));
    world.AddForceGenerator(gravity);

    box.position = {200.0f, 100.0f};
    box.velocity = {0.0f, 0.0f};
    box.size = {50.0f, 50.0f};
    // Initialize collider before accessing it
    box.collider = new Collider(new AABBShape(Vector2(box.size/2)));
    box.collider->restitution = 0.6f; // Set some bounciness
    world.AddBody(&box);

    ground.position = {600.0f, 775.0f};
    ground.size = {1200.0f, 50.0f};
    // Initialize collider before accessing it
    ground.collider = new Collider(new AABBShape(Vector2(ground.size/2)));
    ground.collider->restitution = 0.6f; // Some bounciness for ground
    ground.collider->staticFriction = 0.3f;  // Add friction to ground
    ground.collider->dynamicFriction = 0.2f;
    world.AddBody(&ground);

    ball.position = {400.0f, 50.0f};
    ball.velocity = {0.0f, 0.0f};
    ball.size = {50.0f, 50.0f};
    ball.collider = new Collider(new CircleShape(ball.size.x / 2));
    ball.collider->restitution = 0.9f; // Set some bounciness
    ball.collider->staticFriction = 0.3f;  // Increase friction for more realistic rolling
    ball.collider->dynamicFriction = 0.2f;
    world.AddBody(&ball);
}

// Update the sandbox state
void Sandbox::Update(){
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    // Step the physics world
    world.Step(Time::FixedDeltaTime);
}