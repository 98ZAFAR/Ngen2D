#pragma once
#include "../shapes/Shape.h"
#include <memory>

class Collider{
    public:
        std::unique_ptr<Shape> shape;

        float restitution = 0.5f; // Bounciness factor
        float staticFriction = 0.3f;
        float dynamicFriction = 0.2f;

        explicit Collider(std::unique_ptr<Shape> shape):shape(std::move(shape)) {}
};