#pragma once
#include "../math/Vector2.h"

class RigidBody;

struct CollisionManifold{
    RigidBody* bodyA = nullptr;
    RigidBody* bodyB = nullptr;
    Vector2 normal;
    float penetration = 0.0f;
    int contactCount = 0;
    Vector2 contactPoints[2];
};