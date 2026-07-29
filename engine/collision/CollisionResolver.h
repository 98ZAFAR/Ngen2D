#pragma once
#include "../physics/RigidBody.h"
#include "CollisionManifold.h"

class CollisionResolver{
    public:
        static void ResolveVelocity(const CollisionManifold& manifold);
        static void ResolvePosition(const CollisionManifold& manifold);
};