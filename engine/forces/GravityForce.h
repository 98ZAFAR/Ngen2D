#pragma once
#include "ForceGenerator.h"
#include "../physics/RigidBody.h"

class GravityForce: public ForceGenerator{
    public:
    Vector2 gravity;

    explicit GravityForce(const Vector2& g) : gravity(g) {}

    void Apply(RigidBody& body) override {
        body.ApplyForce(gravity * body.mass);
    }
};