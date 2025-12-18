#include "CollisionResolver.h"
#include <cmath>

void CollisionResolver::Resolve(RigidBody &a, RigidBody &b){
    Vector2 delta = b.position - a.position;

    float overlapX = (a.size.x / 2 + b.size.x / 2) - std::abs(delta.x);
    float overlapY = (a.size.y / 2 + b.size.y / 2) - std::abs(delta.y);

    float totalInverseMass = a.inverseMass + b.inverseMass;
    
    if(totalInverseMass == 0.0f) return;
    
    float aRatio = a.inverseMass / totalInverseMass;
    float bRatio = b.inverseMass / totalInverseMass;

    if(overlapX < overlapY){
        // Adjustment along X axis
        float sign = (delta.x < 0) ? -1.0f : 1.0f;
        a.position.x -= overlapX * sign * aRatio;
        b.position.x += overlapX * sign * bRatio;
    }
    else{
        // Adjustment along Y axis
        float sign = (delta.y < 0) ? -1.0f : 1.0f;
        a.position.y -= overlapY * sign * aRatio;
        b.position.y += overlapY * sign * bRatio;
    }
}