#pragma once
#include "AABBCollider.h"
#include "../shapes/AABBShape.h"
#include "../shapes/CircleShape.h"
#include "../physics/RigidBody.h"
#include "CollisionManifold.h"

class Collision{
    public:
        static AABB GetAABB(const RigidBody &body, const AABBShape &shape);
        static bool AABBvsAABB(const AABB& a, const AABB& b);
        static bool CirclevsCircle(const RigidBody& a,
                                   const RigidBody& b, 
                                   const CircleShape& shapeA, 
                                   const CircleShape& shapeB,
                                   CollisionManifold& manifold);
        static bool AABBvsCircle(const RigidBody& a,
                                  const RigidBody& b,
                                  const AABBShape& shapeA,
                                  const CircleShape& shapeB,
                                  CollisionManifold& manifold);
        static void CheckCollision(RigidBody& a, RigidBody& b);
};