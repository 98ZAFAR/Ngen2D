#include "RigidBody.h"
#include "../shapes/CircleShape.h"

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

    //Update angular velocity from torque
    float angularAcceleration = torque * inverseInertia;
    angularVelocity += angularAcceleration * deltaTime;

    // Apply angular damping after acceleration
    angularVelocity *= angularDamping;
    
    // Clamp small angular velocities to zero
    const float angularEpsilon = 0.05f;
    if (std::abs(angularVelocity) < angularEpsilon)
        angularVelocity = 0.0f;
    
    orientation += angularVelocity * deltaTime;

    // Update velocity from forces
    Vector2 acceleration = force * inverseMass;
    velocity += acceleration * deltaTime;

    // Apply linear damping after acceleration
    velocity *= linearDamping;

    position += velocity * deltaTime;
    // Clear force
    ClearForces();
}

void RigidBody::ClearForces() { 
    force = Vector2(0,0); 
    torque = 0.0f; 
}

void RigidBody::ApplyTorque(float tq){
    torque += tq;
    isSleeping = false;
    sleepTime = 0.0f;
}

void RigidBody::ApplyForceAtPoint(const Vector2& f, const Vector2& point){
    ApplyForce(f);
    Vector2 r = point - position;
    float torqueFromForce = r.cross(f); // 2D cross product
    ApplyTorque(torqueFromForce);
}

void RigidBody::SetInverseInertia(ShapeType type){
    if(type == ShapeType::Circle){
        auto* circle = static_cast<CircleShape*>(collider->shape.get());
        float I = 0.5f * mass * circle->radius * circle->radius;
        if(I > 0)
            inverseInertia = 1.0f / I;
        else
            inverseInertia = 0.0f;
    } else if(type == ShapeType::AABB){
        float width = size.x;
        float height = size.y;
        float I = (1.0f/12.0f) * mass * (width*width + height*height);
        if(I > 0)
            inverseInertia = 1.0f / I;
        else
            inverseInertia = 0.0f;
    } else {
        inverseInertia = 0.0f;
    }
}