#pragma once
#include <cmath>

//Implmentation of a 2D vector
struct Vector2{
    float x, y;

    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}

    // Vector operations
    Vector2 operator+(const Vector2& other) const{
        return Vector2(x+other.x, y+other.y);
    }
    Vector2 operator-(const Vector2& other) const{
        return Vector2(x-other.x, y-other.y);
    }
    Vector2 operator*(float scalar) const{
        return Vector2(x*scalar, y*scalar);
    }
    Vector2 operator/(float scalar) const{
        return Vector2(x/scalar, y/scalar);
    }
    Vector2& operator+=(const Vector2& other){
        x+=other.x;
        y+=other.y;

        return *this;
    }

    Vector2& operator-=(const Vector2& other){
        x-=other.x;
        y-=other.y;

        return *this;
    }
    Vector2& operator*=(float scalar){
        x*=scalar;
        y*=scalar;

        return *this;
    }
    Vector2& operator/=(float scalar){
        x/=scalar;
        y/=scalar;

        return *this;
    }

    // Calculate Dot Product
    float dot(const Vector2& other) const{
        return x*other.x + y*other.y;
    }

    float cross(const Vector2& other) const{
        return x*other.y - y*other.x;
    }

    // Calculate squared magnitude (faster, avoids sqrt)
    float lengthSquared() const{
        return x*x + y*y;
    }

    // Calculate Magnitude
    float length() const{
        return std::sqrt(lengthSquared());
    }

    // Normalize the vector
    Vector2 normalize() const {
        float mgn = length();
        if(mgn>0.00001f) return Vector2(x/mgn, y/mgn);
        return Vector2(0, 0);
    }
};