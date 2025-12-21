#include "PhysicsWorld.h"

#include "../core/Config.h"
#include "../collision/Collision.h"
#include "../collision/CollisionResolver.h"

void PhysicsWorld::AddBody(RigidBody* body){
    bodies.push_back(body);
}

void PhysicsWorld::Step(float deltaTime){
    // Apply gravity and integrate motion
    for(auto body : bodies){
        if(body->isSleeping) continue;

        body->ApplyGravity();
        body->Integrate(deltaTime);

        // Sleep detection (use lengthSquared to avoid sqrt)
        float sleepThresholdSq = Config::SleepVelocityThreshold * Config::SleepVelocityThreshold;
        if(body->velocity.lengthSquared() < sleepThresholdSq){
            body->sleepTime += deltaTime;
            if(body->sleepTime >= Config::SleepTimeThreshold){
                body->isSleeping = true;
                body->velocity = Vector2(0,0);
            }
        } else {
            body->sleepTime = 0.0f;
        }
    }

    // Collision detection and resolution
    for(int it = 0; it < iterations; it++){
        if (useSpatialHash) { // Use spatial hashing
            spatialHash.Clear();
            
            for(int i = 0; i < bodies.size(); i++){
                spatialHash.Insert(bodies[i], i);
            }
            
            auto pairs = spatialHash.GetPotentialCollisions(bodies);
            
            for(const auto& pair : pairs){
                RigidBody* bodyA = bodies[pair.first];
                RigidBody* bodyB = bodies[pair.second];
                
                if(bodyA->isSleeping && bodyB->isSleeping) continue;
                if(bodyA->inverseMass == 0.0f && bodyB->inverseMass == 0.0f) continue;
                
                Collision::CheckCollision(*bodyA, *bodyB);
            }
        } else { // Brute-force check
            for(int i = 0; i < bodies.size(); i++){
                RigidBody* bodyA = bodies[i];
                
                for(int j = i + 1; j < bodies.size(); j++){
                    RigidBody* bodyB = bodies[j];
                    
                    if(bodyA->isSleeping && bodyB->isSleeping) continue;
                    if(bodyA->inverseMass == 0.0f && bodyB->inverseMass == 0.0f) continue;
                    
                    Collision::CheckCollision(*bodyA, *bodyB);
                }
            }
        }
    }
}