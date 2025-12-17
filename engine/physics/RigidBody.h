#pragma once
#include "../math/Vector2.h"

class RigidBody{
    public:
    Vector2 position;
    Vector2 velocity;
    Vector2 force;

    float mass;
    float inverseMass;

    RigidBody(float m=1.0f);

    void ApplyForce(const Vector2& f);
    void Integrate(float deltaTime);
};