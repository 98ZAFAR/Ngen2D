#pragma once
#include "../physics/RigidBody.h"
#include "CollisionManifold.h"

class CollisionResolver{
    public:
        static void Resolve(RigidBody &a, RigidBody &b, const CollisionManifold& manifold);
};