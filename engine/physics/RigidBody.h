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
    float inverseInertia=0.0f;

    float orientation = 0.0f; // radians
    float angularVelocity = 0.0f;
    float torque = 0.0f;

    bool isSleeping = false;
    float sleepTime = 0.0f;

    float linearDamping = 0.995f;
    float angularDamping = 0.96f; // Stronger damping to stop spinning faster

    Collider* collider = nullptr;

    RigidBody(float m=1.0f);

    void ApplyForce(const Vector2& f);
    void ApplyForceAtPoint(const Vector2& f, const Vector2& point);
    void ApplyTorque(float t);
    void Integrate(float deltaTime);
    void SetInverseInertia(ShapeType type);
    void ClearForces();
};