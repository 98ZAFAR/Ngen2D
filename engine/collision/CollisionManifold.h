#pragma once
#include "../math/Vector2.h"

struct CollisionManifold{
    Vector2 normal;
    float penetration;
    Vector2 contactPoint;
};