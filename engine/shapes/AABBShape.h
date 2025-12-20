#pragma once
#include "Shape.h"
#include "../math/Vector2.h"


class AABBShape : public Shape {
    public:
        Vector2 halfsize;
        
        AABBShape(const Vector2& halfsize) : Shape(ShapeType::AABB), halfsize(halfsize) {}
};