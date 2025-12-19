#pragma once
#include "../engine/physics/PhysicsWorld.h"

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
        RigidBody ground;
};