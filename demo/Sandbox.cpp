#include "Sandbox.h"
#include "../engine/core/Time.h"
#include "../engine/collision/Collision.h"
#include "../engine/collision/CollisionResolver.h"

#include <iostream>
#include <thread>
#include <chrono>

// Initialize the sandbox with a box and ground
Sandbox::Sandbox(): box(1.0f), ground(0.0f) {
    box.position = {200.0f, 100.0f};
    box.size = {50.0f, 50.0f};
    box.restitution = 0.8f; // Set some bounciness
    world.AddBody(&box);

    ground.position = {600.0f, 775.0f};
    ground.size = {1200.0f, 50.0f};
    ground.restitution = 0.5f; // Some bounciness for ground
    world.AddBody(&ground);
}

// Update the sandbox state
void Sandbox::Update(){
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    // Step the physics world
    world.Step(Time::FixedDeltaTime);

    // float rectHeight = box.size.y/2.0f;
    // float groundTop = ground.position.y - ground.size.y/2.0f;
    // if(box.position.y+rectHeight >= groundTop){
    //     box.position.y = groundTop - rectHeight;
    //     box.velocity.y = 0;
    //     // box.ApplyForce(Vector2(0, -2*box.mass * 980.0f)); // Apply an upward force to simulate bounce
    // }

    // Simple check for collisions between all bodies
    for(int i=0; i<world.GetBodyCount(); i++){
        RigidBody* bodyA = world.GetBody(i);
        AABB aabbA = Collision::GetAABB(*bodyA);

        for(int j=i+1; j<world.GetBodyCount(); j++){
            RigidBody* bodyB = world.GetBody(j);
            AABB aabbB = Collision::GetAABB(*bodyB);

            if(Collision::AABBvsAABB(aabbA, aabbB)){
                CollisionResolver::Resolve(*bodyA, *bodyB);
            }
        }
    }
}