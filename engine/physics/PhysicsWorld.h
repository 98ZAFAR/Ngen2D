#pragma once
#include<vector>
#include "RigidBody.h"

class PhysicsWorld{
    public:
        void AddBody(RigidBody* body);
        void Step(float deltaTime);
    private:
        // Internal data structures for physics bodies would go here
        std::vector<RigidBody*> bodies;
};