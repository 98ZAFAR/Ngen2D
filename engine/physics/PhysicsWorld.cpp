#include "PhysicsWorld.h"

void PhysicsWorld::AddBody(RigidBody* body){
    bodies.push_back(body);
}

void PhysicsWorld::Step(float deltaTime){
    for(auto body : bodies){
        body->ApplyGravity();
        body->Integrate(deltaTime);
    }
}