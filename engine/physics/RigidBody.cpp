#include "RigidBody.h"
#include "../core/Config.h"

RigidBody::RigidBody(float m):mass(m), position(), velocity(), force(){
    if(mass > 0){
        inverseMass = 1.0f / mass;
    } else {
        inverseMass = 0.0f; // Infinite mass
    }
}

void RigidBody::ApplyForce(const Vector2& f){
    force += f;
}

void RigidBody::Integrate(float deltaTime){
    if(inverseMass<=0.0f) return;

    // Update velocity
    Vector2 acceleration = force * inverseMass;
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    // Clear force
    force = Vector2(0,0);
}

void RigidBody::ApplyGravity(){
    if(inverseMass<=0.0f) return;
    ApplyForce(Vector2(0, Config::GRAVITY * mass));
}