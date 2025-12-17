#include "Sandbox.h"
#include "../engine/core/Time.h"

#include <iostream>
#include <thread>
#include <chrono>

Sandbox::Sandbox(): box(1.0f) {
    box.position = {200.0f, 100.0f}; // Start near the top of the window
    world.AddBody(&box);
}

void Sandbox::Update(){
    // For demonstration, print the box position each update
    std::cout << "Box Position: (" << box.position.x << ", " << box.position.y << ")\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    // Step the physics world
    world.Step(Time::FixedDeltaTime);
}

RigidBody* Sandbox::GetBox(){
    return &box;
}