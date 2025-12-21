#include "RigidBody.h"

RigidBody::RigidBody(float m):mass(m), position(), velocity(), force(){
    if(mass > 0)
        inverseMass = 1.0f / mass;
    else
        inverseMass = 0.0f;
}

void RigidBody::ApplyForce(const Vector2& f){
    sleepTime = 0.0f;
    isSleeping = false;
    force += f;
}

void RigidBody::Integrate(float deltaTime){
    if(inverseMass<=0.0f) return;

    if(isSleeping) return;
    
    // Apply air damping
    velocity *= linearDamping;

    // Update velocity
    Vector2 acceleration = force * inverseMass;
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    // Clear force
    ClearForces();
}

void RigidBody::ClearForces() { force = Vector2(0,0); }


void RigidBody::ApplyTorque(float torque){
}