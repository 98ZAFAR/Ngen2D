#pragma once
#include "AABBCollider.h"
#include "../shapes/AABBShape.h"
#include "../shapes/CircleShape.h"
#include "../physics/RigidBody.h"
#include "CollisionManifold.h"

class Collision{
    public:
        static AABB GetAABB(const RigidBody &body, const AABBShape &shape);
        static bool AABBvsAABB(const RigidBody &a, 
                               const RigidBody &b,
                               CollisionManifold& manifold);
        static bool OBBvsOBB(const RigidBody &a, 
                             const RigidBody &b,
                             CollisionManifold& manifold);
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
        static bool OBBvsCircle(const RigidBody& a,
                                const RigidBody& b,
                                const AABBShape& shapeA,
                                const CircleShape& shapeB,
                                CollisionManifold& manifold);
        static void CheckCollision(RigidBody& a, RigidBody& b);
    private:
        static void GetOBBCorners(const RigidBody& body, const AABBShape& shape, Vector2 corners[4]);
        static float ProjectOntoAxis(const Vector2 corners[4], int numCorners, const Vector2& axis, float& min, float& max);
};