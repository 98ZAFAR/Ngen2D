#pragma once
#include "AABBCollider.h"
#include "../physics/RigidBody.h"

class Collision{
    public:
        static AABB GetAABB(const RigidBody &body);
        static bool AABBvsAABB(const AABB& a, const AABB& b);
};