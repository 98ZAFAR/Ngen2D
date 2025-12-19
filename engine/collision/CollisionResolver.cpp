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

    if(overlapX>0 && overlapY>0){
        if(overlapX < overlapY){
            float separation = (delta.x < 0) ? -overlapX : overlapX;
            a.position.x -= separation * aRatio;
            b.position.x += separation * bRatio;

            a.velocity.x = 0;
            b.velocity.x = 0;
        } else {
            float separation = (delta.y < 0) ? -overlapY : overlapY;
            a.position.y -= separation * aRatio;
            b.position.y += separation * bRatio;

            a.velocity.y = 0;
            b.velocity.y = 0;
        }
    }
}