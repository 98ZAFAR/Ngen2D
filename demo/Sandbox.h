#pragma once
#include "../engine/physics/PhysicsWorld.h"
#include <chrono>

class Sandbox{
    public:
        Sandbox();
        void Update();
        RigidBody* GetBox() { return box; };
        RigidBody* GetGround() { return ground; };
        PhysicsWorld& GetWorld() { return world; }
    private:
        PhysicsWorld world;
        RigidBody* box = nullptr;      // non-owning, world owns the body
        RigidBody* ground = nullptr;   // non-owning, world owns the body
        
        // Fixed timestep variables
        float accumulator = 0.0f;
        std::chrono::high_resolution_clock::time_point lastTime;
};