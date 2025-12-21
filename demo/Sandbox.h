#pragma once
#include "../engine/physics/PhysicsWorld.h"
#include <chrono>

class Sandbox{
    public:
        Sandbox();
        void Update();
        RigidBody* GetBox() { return &box; };
        RigidBody* GetGround() { return &ground; };
        PhysicsWorld& GetWorld() { return world; }
    private:
        // Sandbox specific data and methods would go here
        PhysicsWorld world;
        RigidBody box;
        RigidBody ball;
        RigidBody ground;
        
        // Fixed timestep variables
        float accumulator = 0.0f;
        std::chrono::high_resolution_clock::time_point lastTime;
};