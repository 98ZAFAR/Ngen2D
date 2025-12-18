#pragma once
#include<vector>
#include "RigidBody.h"

class PhysicsWorld{
    public:
        void AddBody(RigidBody* body);
        void Step(float deltaTime);
        int GetBodyCount() const { return bodies.size(); }
        RigidBody* GetBody(int index) const { return bodies[index]; }
    private:
        // Internal data structures for physics bodies would go here
        std::vector<RigidBody*> bodies;
};