#include "PhysicsWorld.h"

#include "../collision/Collision.h"
#include "../collision/CollisionResolver.h"

void PhysicsWorld::AddBody(RigidBody* body){
    bodies.push_back(body);
}

void PhysicsWorld::Step(float deltaTime){
    // Apply gravity and integrate motion
    for(auto body : bodies){
        body->ApplyGravity();
        body->Integrate(deltaTime);
    }

    // Check for collisions
    int iterations = 8; // Number of collision resolution iterations
    for(int it=0; it<iterations; it++){
        for(int i=0; i<bodies.size(); i++){
            RigidBody* bodyA = bodies[i];
            AABB aabbA = Collision::GetAABB(*bodyA);

            for(int j=i+1; j<bodies.size(); j++){
                RigidBody* bodyB = bodies[j];
                AABB aabbB = Collision::GetAABB(*bodyB);

                if(Collision::AABBvsAABB(aabbA, aabbB)){
                    CollisionResolver::Resolve(*bodyA, *bodyB);
                }
            }
        }
    }
}