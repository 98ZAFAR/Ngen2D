#pragma once
#include "../math/Vector2.h"
#include "../collision/Collider.h"

class RigidBody{
    public:
    Vector2 size;
    Vector2 position;
    Vector2 velocity;
    Vector2 force;

    float mass;
    float inverseMass;

    Collider* collider = nullptr;

    RigidBody(float m=1.0f);

    void ApplyForce(const Vector2& f);
    void ApplyTorque(float torque);
    void Integrate(float deltaTime);
    void ApplyGravity();
};