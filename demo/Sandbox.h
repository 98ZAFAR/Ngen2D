#pragma once
#include "../engine/physics/PhysicsWorld.h"

class Sandbox{
    public:
        Sandbox();
        void Update();
        RigidBody* GetBox();
        RigidBody* GetGround();
    private:
        // Sandbox specific data and methods would go here
        PhysicsWorld world;
        RigidBody box;
        RigidBody ground;
};