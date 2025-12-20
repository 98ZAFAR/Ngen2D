#pragma once
#include "Shape.h"

class CircleShape : public Shape {
    public:
        float radius;
        CircleShape(float radius) : Shape(ShapeType::Circle), radius(radius) {}
};