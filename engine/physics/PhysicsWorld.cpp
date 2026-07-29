#include "PhysicsWorld.h"

#include "../core/Config.h"
#include "../collision/Collision.h"
#include "../collision/CollisionResolver.h"
#include <algorithm>

void PhysicsWorld::AddBody(std::unique_ptr<RigidBody> body){
    bodies.push_back(std::move(body));
}

void PhysicsWorld::RemoveBody(RigidBody* body){
    bodies.erase(
        std::remove_if(bodies.begin(), bodies.end(),
            [body](const std::unique_ptr<RigidBody>& b){ return b.get() == body; }),
        bodies.end());
}

void PhysicsWorld::AddForceGenerator(std::unique_ptr<ForceGenerator> fg){
    forceGenerators.push_back(std::move(fg));
}

void PhysicsWorld::Step(float deltaTime){
    // Apply Force Generators
    for(auto& fg : forceGenerators){
        for(auto& body : bodies){
            if(!body->isSleeping)
                fg->Apply(*body);
        }
    }

    // Integrate motion
    for(auto& body : bodies){
        if(body->isSleeping) continue;
        body->Integrate(deltaTime);
    }

    // 1. Broadphase & Narrowphase: Find all collisions once
    std::vector<CollisionManifold> contacts;
    if (useSpatialHash) { // Use spatial hashing
        spatialHash.Clear();
        
        for(int i = 0; i < bodies.size(); i++){
            spatialHash.Insert(bodies[i].get(), i);
        }
        
        auto pairs = spatialHash.GetPotentialCollisions();
        
        for(const auto& pair : pairs){
            RigidBody* bodyA = bodies[pair.first].get();
            RigidBody* bodyB = bodies[pair.second].get();
            
            if(bodyA->isSleeping && bodyB->isSleeping) continue;
            if(bodyA->inverseMass == 0.0f && bodyB->inverseMass == 0.0f) continue;
            
            CollisionManifold m;
            if (Collision::CheckCollision(*bodyA, *bodyB, m)) {
                contacts.push_back(m);
            }
        }
    } else { // Brute-force check
        for(int i = 0; i < bodies.size(); i++){
            RigidBody* bodyA = bodies[i].get();
            
            for(int j = i + 1; j < bodies.size(); j++){
                RigidBody* bodyB = bodies[j].get();
                
                if(bodyA->isSleeping && bodyB->isSleeping) continue;
                if(bodyA->inverseMass == 0.0f && bodyB->inverseMass == 0.0f) continue;
                
                CollisionManifold m;
                if (Collision::CheckCollision(*bodyA, *bodyB, m)) {
                    contacts.push_back(m);
                }
            }
        }
    }

    // 2. Resolve Velocities (Impulses)
    for(int it = 0; it < velocityIterations; it++){
        for(const auto& contact : contacts){
            CollisionResolver::ResolveVelocity(contact);
        }
    }

    // 3. Resolve Positions (Baumgarte Stabilization)
    for(int it = 0; it < positionIterations; it++){
        for(const auto& contact : contacts){
            CollisionResolver::ResolvePosition(contact);
        }
    }

    // Sleep detection (post-collision resolution so resting velocity is accurate)
    for(auto& body : bodies){
        if(body->isSleeping) continue;

        float sleepThresholdSq = Config::SleepVelocityThreshold * Config::SleepVelocityThreshold;
        bool isLinearSlow = body->velocity.lengthSquared() < sleepThresholdSq;
        bool isAngularSlow = std::abs(body->angularVelocity) < Config::SleepVelocityThreshold;
        
        if(isLinearSlow && isAngularSlow){
            body->sleepTime += deltaTime;
            if(body->sleepTime >= Config::SleepTimeThreshold){
                body->isSleeping = true;
                body->velocity = Vector2(0,0);
                body->angularVelocity = 0.0f;
            }
        } else {
            body->sleepTime = 0.0f;
        }
    }
}