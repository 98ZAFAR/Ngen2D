#include "Collision.h"

AABB Collision::GetAABB(const RigidBody &body){
    AABB aabb;
    aabb.min = Vector2(body.position.x - body.size.x / 2, body.position.y - body.size.y / 2);
    aabb.max = Vector2(body.position.x + body.size.x / 2, body.position.y + body.size.y / 2);
    return aabb;
}

bool Collision::AABBvsAABB(const AABB& a, const AABB& b){
    // Check for overlap on x and y axes
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
           (a.min.y <= b.max.y && a.max.y >= b.min.y);
}