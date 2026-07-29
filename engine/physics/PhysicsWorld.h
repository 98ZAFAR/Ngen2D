#pragma once
#include<vector>
#include<memory>
#include "RigidBody.h"
#include "../forces/ForceGenerator.h"
#include "SpatialHash.h"

class PhysicsWorld{
    public:
        void AddBody(std::unique_ptr<RigidBody> body);
        void RemoveBody(RigidBody* body);
        void AddForceGenerator(std::unique_ptr<ForceGenerator> fg);
        void Step(float deltaTime);
        int GetBodyCount() const { return bodies.size(); }
        RigidBody* GetBody(int index) const { return bodies[index].get(); }
        
        // Performance settings
        void SetVelocityIterations(int iterations) { this->velocityIterations = iterations; }
        void SetPositionIterations(int iterations) { this->positionIterations = iterations; }
        void SetUseSpatialHash(bool use) { useSpatialHash = use; }
        
    private:
        // Internal data structures for physics bodies would go here
        std::vector<std::unique_ptr<RigidBody>> bodies;
        std::vector<std::unique_ptr<ForceGenerator>> forceGenerators;
        SpatialHash spatialHash;
        
        // Performance settings
        int velocityIterations = 8;
        int positionIterations = 3;
        bool useSpatialHash = true;
};