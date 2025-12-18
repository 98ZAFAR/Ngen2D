#pragma once
#include "../physics/RigidBody.h"

class CollisionResolver{
    public:
        static void Resolve(RigidBody &a, RigidBody &b);
};