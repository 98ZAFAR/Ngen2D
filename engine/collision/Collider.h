#pragma once
#include "../shapes/Shape.h"

class Collider{
    public:
        Shape* shape;

        float restitution = 0.5f; // Bounciness factor
        float staticFriction = 0.3f;
        float dynamicFriction = 0.2f;

        explicit Collider(Shape* shape):shape(shape) {}
};