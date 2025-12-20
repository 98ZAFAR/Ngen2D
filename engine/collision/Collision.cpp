#include "Collision.h"
#include "CollisionResolver.h"
#include "../math/MathUtils.h"

// Get the Axis-Aligned Bounding Box for a RigidBody with an AABBShape
AABB Collision::GetAABB(const RigidBody &body, const AABBShape &shape)
{
    AABB aabb;
    aabb.min = body.position - shape.halfsize;
    aabb.max = body.position + shape.halfsize;
    return aabb;
}

// Check collision between two AABBs
bool Collision::AABBvsAABB(const AABB &a, const AABB &b)
{
    // Check for overlap on x and y axes
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
           (a.min.y <= b.max.y && a.max.y >= b.min.y);
}

// Check collision between two circles
bool Collision::CirclevsCircle(const RigidBody &a,
                               const RigidBody &b,
                               const CircleShape &shapeA,
                               const CircleShape &shapeB,
                               CollisionManifold &manifold)
{
    Vector2 delta = b.position - a.position;
    float radiiSum = shapeA.radius + shapeB.radius;

    float distSq = delta.dot(delta);

    if (distSq >= radiiSum * radiiSum)
        return false;

    float dist = std::sqrt(distSq);
    if (dist != 0.0f)
        manifold.normal = delta / dist;
    else
        manifold.normal = Vector2(1.0f, 0.0f);

    manifold.penetration = radiiSum - dist;
    return true;
}

// Check collision between AABB and Circle
bool Collision::AABBvsCircle(const RigidBody &a,
                                  const RigidBody &b,
                                  const AABBShape &shapeA,
                                  const CircleShape &shapeB,
                                  CollisionManifold &manifold)
{
    Vector2 aabbCenter = a.position;
    Vector2 circleCenter = b.position;

    Vector2 difference = circleCenter - aabbCenter;
    Vector2 clamped = Vector2(
        Clamp(difference.x, -shapeA.halfsize.x, shapeA.halfsize.x),
        Clamp(difference.y, -shapeA.halfsize.y, shapeA.halfsize.y)
    );

    Vector2 closest = aabbCenter + clamped;
    difference = closest - circleCenter;

    float distanceSq = difference.dot(difference);
    if (distanceSq > (shapeB.radius * shapeB.radius))
        return false;

    float distance = std::sqrt(distanceSq);
    if (distance != 0.0f)
    {
        // Normal points from AABB to Circle (circle - closest point)
        manifold.normal = (circleCenter - closest) / distance;
    }
    else
    {
        // Circle center is inside AABB, use clamped direction
        if (clamped.x == 0.0f && clamped.y == 0.0f)
            manifold.normal = Vector2(0.0f, -1.0f);
        else
            manifold.normal = clamped.normalize();
    }

    manifold.penetration = shapeB.radius - distance;
    return true;
}

// Main collision checking function
void Collision::CheckCollision(RigidBody &a, RigidBody &b)
{
    ShapeType typeA = a.collider ? a.collider->shape->GetType() : ShapeType::AABB;
    ShapeType typeB = b.collider ? b.collider->shape->GetType() : ShapeType::AABB;

    if (typeA == ShapeType::AABB && typeB == ShapeType::AABB)
    {
        auto *shapeA = static_cast<AABBShape *>(a.collider->shape);
        auto *shapeB = static_cast<AABBShape *>(b.collider->shape);

        AABB aabbA = GetAABB(a, *shapeA);
        AABB aabbB = GetAABB(b, *shapeB);

        if (AABBvsAABB(aabbA, aabbB))
        {
            CollisionResolver::Resolve(a, b);
        }
    }
    else if (typeA == ShapeType::Circle && typeB == ShapeType::Circle)
    {
        auto *shapeA = static_cast<CircleShape *>(a.collider->shape);
        auto *shapeB = static_cast<CircleShape *>(b.collider->shape);

        CollisionManifold manifold;
        if (CirclevsCircle(a, b, *shapeA, *shapeB, manifold))
        {
            CollisionResolver::Resolve(a, b, manifold);
        }
    }
    else if (typeA == ShapeType::Circle && typeB == ShapeType::AABB)
    {
        auto *circle = static_cast<CircleShape *>(a.collider->shape);
        auto *box = static_cast<AABBShape *>(b.collider->shape);

        CollisionManifold m;
        if (AABBvsCircle(b, a, *box, *circle, m))
        {
            m.normal = m.normal * -1.0f;
            CollisionResolver::Resolve(a, b, m);
        }
    }
    else if (typeA == ShapeType::AABB && typeB == ShapeType::Circle)
    {
        auto *box = static_cast<AABBShape *>(a.collider->shape);
        auto *circle = static_cast<CircleShape *>(b.collider->shape);

        CollisionManifold m;
        if (AABBvsCircle(a, b, *box, *circle, m))
            CollisionResolver::Resolve(a, b, m);
    }
}