#pragma once
#include<vector>
#include "RigidBody.h"
#include "../forces/ForceGenerator.h"
#include "SpatialHash.h"

class PhysicsWorld{
    public:
        void AddBody(RigidBody* body);
        void AddForceGenerator(class ForceGenerator* fg);
        void Step(float deltaTime);
        int GetBodyCount() const { return bodies.size(); }
        RigidBody* GetBody(int index) const { return bodies[index]; }
        
        // Performance settings
        void SetIterations(int iterations) { this->iterations = iterations; }
        void SetUseSpatialHash(bool use) { useSpatialHash = use; }
        
    private:
        // Internal data structures for physics bodies would go here
        std::vector<RigidBody*> bodies;
        std::vector<ForceGenerator*> forceGenerators;
        SpatialHash spatialHash;
        
        // Performance settings
        int iterations = 4; // Reduced from 8
        bool useSpatialHash = true;
};