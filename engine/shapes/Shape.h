#pragma once

enum class ShapeType {
    Circle,
    AABB
};

class Shape{
    public:
        Shape(ShapeType type) : type(type) {}
        virtual ~Shape() = default;
        ShapeType GetType() const { return type; }

    private:
        ShapeType type;
};