#pragma once
#include "../math/Vector2.h"

class RigidBody{
    public:
    Vector2 size;
    Vector2 position;
    Vector2 velocity;
    Vector2 force;

    float mass;
    float inverseMass;
    float restitution = 0.0f; // Bounciness factor
    float staticFriction = 0.3f;
    float dynamicFriction = 0.2f;

    RigidBody(float m=1.0f);

    void ApplyForce(const Vector2& f);
    void ApplyTorque(float torque);
    void Integrate(float deltaTime);
    void ApplyGravity();
};