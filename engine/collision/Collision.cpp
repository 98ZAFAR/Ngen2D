#include "Collision.h"
#include "CollisionResolver.h"
#include "../math/MathUtils.h"
#include <vector>

// Get the Axis-Aligned Bounding Box for a RigidBody with an AABBShape
AABB Collision::GetAABB(const RigidBody &body, const AABBShape &shape)
{
    AABB aabb;
    aabb.min = body.position - shape.halfsize;
    aabb.max = body.position + shape.halfsize;
    return aabb;
}

// Get the 4 corners of an Oriented Bounding Box
void Collision::GetOBBCorners(const RigidBody& body, const AABBShape& shape, Vector2 corners[4])
{
    float cosA = std::cos(body.orientation);
    float sinA = std::sin(body.orientation);
    
    Vector2 halfsize = shape.halfsize;
    
    // Local space corners
    Vector2 localCorners[4] = {
        Vector2(-halfsize.x, -halfsize.y),
        Vector2(halfsize.x, -halfsize.y),
        Vector2(halfsize.x, halfsize.y),
        Vector2(-halfsize.x, halfsize.y)
    };
    
    // Rotate and translate to world space
    for (int i = 0; i < 4; i++)
    {
        float x = localCorners[i].x;
        float y = localCorners[i].y;
        corners[i].x = body.position.x + (x * cosA - y * sinA);
        corners[i].y = body.position.y + (x * sinA + y * cosA);
    }
}

// Project corners onto an axis and return the min/max projection values
float Collision::ProjectOntoAxis(const Vector2 corners[], int numCorners, const Vector2& axis, float& min, float& max)
{
    min = max = corners[0].dot(axis);
    
    for (int i = 1; i < numCorners; i++)
    {
        float projection = corners[i].dot(axis);
        if (projection < min) min = projection;
        if (projection > max) max = projection;
    }
    
    return max - min;
}

// Check collision between two Oriented Bounding Boxes using SAT
bool Collision::OBBvsOBB(const RigidBody &a, const RigidBody &b, CollisionManifold& manifold)
{
    AABBShape *shapeA = static_cast<AABBShape *>(a.collider->shape);
    AABBShape *shapeB = static_cast<AABBShape *>(b.collider->shape);
    
    // Get corners of both OBBs
    Vector2 cornersA[4], cornersB[4];
    GetOBBCorners(a, *shapeA, cornersA);
    GetOBBCorners(b, *shapeB, cornersB);
    
    // Get axes to test (perpendicular to edges)
    Vector2 axes[4];
    
    // Axes from box A (perpendicular to edges)
    float cosA = std::cos(a.orientation);
    float sinA = std::sin(a.orientation);
    axes[0] = Vector2(cosA, sinA);           // Right direction
    axes[1] = Vector2(-sinA, cosA);          // Up direction
    
    // Axes from box B
    float cosB = std::cos(b.orientation);
    float sinB = std::sin(b.orientation);
    axes[2] = Vector2(cosB, sinB);
    axes[3] = Vector2(-sinB, cosB);
    
    float minOverlap = FLT_MAX;
    Vector2 smallestAxis;
    
    // Test all axes using SAT
    for (int i = 0; i < 4; i++)
    {
        float minA, maxA, minB, maxB;
        ProjectOntoAxis(cornersA, 4, axes[i], minA, maxA);
        ProjectOntoAxis(cornersB, 4, axes[i], minB, maxB);
        
        // Check for separation
        if (maxA < minB || maxB < minA)
            return false; // No collision
        
        // Calculate overlap
        float overlap = std::min(maxA, maxB) - std::max(minA, minB);
        
        if (overlap < minOverlap)
        {
            minOverlap = overlap;
            smallestAxis = axes[i];
            
            // Make sure normal points from A to B
            Vector2 centerDiff = b.position - a.position;
            if (centerDiff.dot(smallestAxis) < 0)
                smallestAxis = smallestAxis * -1.0f;
        }
    }
    
    // Collision detected, fill manifold
    manifold.normal = smallestAxis;
    manifold.penetration = minOverlap;
    
    // Find contact points: vertices of one box that are inside the other box
    std::vector<Vector2> contactPoints;
    
    // Check vertices of B that are "inside" A (behind the collision normal)
    for (int i = 0; i < 4; i++)
    {
        // Project B's corner onto the collision normal from A's center
        Vector2 diff = cornersB[i] - a.position;
        float projection = diff.dot(manifold.normal);
        
        // If the projection is positive and within the penetration depth, it's a contact point
        if (projection > 0 && projection <= minOverlap + 0.1f)
        {
            contactPoints.push_back(cornersB[i]);
        }
    }
    
    // Check vertices of A that are "inside" B (ahead of the collision normal)
    for (int i = 0; i < 4; i++)
    {
        Vector2 diff = cornersA[i] - b.position;
        float projection = diff.dot(manifold.normal);
        
        // If the projection is negative and within the penetration depth, it's a contact point
        if (projection < 0 && std::abs(projection) <= minOverlap + 0.1f)
        {
            contactPoints.push_back(cornersA[i]);
        }
    }
    
    // Use average of contact points, or center between objects if none found
    if (contactPoints.size() > 0)
    {
        Vector2 avgContact(0, 0);
        for (const auto& point : contactPoints)
        {
            avgContact = avgContact + point;
        }
        manifold.contactPoint = avgContact / static_cast<float>(contactPoints.size());
    }
    else
    {
        // Fallback: contact at the midpoint between centers projected onto collision normal
        manifold.contactPoint = a.position + manifold.normal * (shapeA->halfsize.x + shapeA->halfsize.y) * 0.5f;
    }
    
    return true;
}

// Check collision between two AABBs
bool Collision::AABBvsAABB(const RigidBody &a, const RigidBody &b, CollisionManifold& manifold)
{
    // ---- early out ----
    Vector2 delta = b.position - a.position;

    AABBShape *shapeA = static_cast<AABBShape *>(a.collider->shape);
    AABBShape *shapeB = static_cast<AABBShape *>(b.collider->shape);

    float overlapX = (shapeA->halfsize.x + shapeB->halfsize.x) - std::abs(delta.x);
    float overlapY = (shapeA->halfsize.y + shapeB->halfsize.y) - std::abs(delta.y);

    if (overlapX <= 0.0f || overlapY <= 0.0f)
        return false;

    // ---- collision normal ----

    if (overlapX < overlapY)
    {
        manifold.normal = {(delta.x < 0) ? -1.0f : 1.0f, 0.0f};
        manifold.penetration = overlapX;
        // Contact point on the edge between the two boxes
        float contactX = (delta.x < 0) ? (a.position.x - shapeA->halfsize.x) : (a.position.x + shapeA->halfsize.x);
        float contactY = a.position.y + Clamp(delta.y, -shapeA->halfsize.y, shapeA->halfsize.y);
        manifold.contactPoint = Vector2(contactX, contactY);
    }
    else
    {
        manifold.normal = {0.0f, (delta.y < 0) ? -1.0f : 1.0f};
        manifold.penetration = overlapY;
        // Contact point on the edge between the two boxes
        float contactX = a.position.x + Clamp(delta.x, -shapeA->halfsize.x, shapeA->halfsize.x);
        float contactY = (delta.y < 0) ? (a.position.y - shapeA->halfsize.y) : (a.position.y + shapeA->halfsize.y);
        manifold.contactPoint = Vector2(contactX, contactY);
    }

    AABB aabbA = GetAABB(a, *shapeA);
    AABB aabbB = GetAABB(b, *shapeB);
    // Check for overlap on x and y axes
    return (aabbA.min.x <= aabbB.max.x && aabbA.max.x >= aabbB.min.x) &&
           (aabbA.min.y <= aabbB.max.y && aabbA.max.y >= aabbB.min.y);
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
    // Contact point is on the surface of circle A along the collision normal
    manifold.contactPoint = a.position + manifold.normal * shapeA.radius;
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
    // Contact point is the closest point on the AABB to the circle
    manifold.contactPoint = closest;
    return true;
}

// Check collision between OBB and Circle
bool Collision::OBBvsCircle(const RigidBody& a,
                            const RigidBody& b,
                            const AABBShape& shapeA,
                            const CircleShape& shapeB,
                            CollisionManifold& manifold)
{
    // Transform circle center to OBB local space
    Vector2 diff = b.position - a.position;
    
    float cosA = std::cos(-a.orientation);
    float sinA = std::sin(-a.orientation);
    
    // Rotate circle center to box's local space
    Vector2 localCircle;
    localCircle.x = diff.x * cosA - diff.y * sinA;
    localCircle.y = diff.x * sinA + diff.y * cosA;
    
    // Clamp to box bounds in local space
    Vector2 clamped = Vector2(
        Clamp(localCircle.x, -shapeA.halfsize.x, shapeA.halfsize.x),
        Clamp(localCircle.y, -shapeA.halfsize.y, shapeA.halfsize.y)
    );
    
    // Find closest point in local space
    Vector2 localClosest = clamped;
    
    // Transform back to world space
    cosA = std::cos(a.orientation);
    sinA = std::sin(a.orientation);
    Vector2 closest;
    closest.x = a.position.x + (localClosest.x * cosA - localClosest.y * sinA);
    closest.y = a.position.y + (localClosest.x * sinA + localClosest.y * cosA);
    
    // Check distance
    Vector2 difference = closest - b.position;
    float distanceSq = difference.dot(difference);
    
    if (distanceSq > (shapeB.radius * shapeB.radius))
        return false;
    
    float distance = std::sqrt(distanceSq);
    
    if (distance > 1e-6f)
    {
        manifold.normal = (b.position - closest) / distance;
    }
    else
    {
        // Circle center is at closest point, use direction from OBB center
        Vector2 dir = b.position - a.position;
        float len = dir.length();
        if (len > 1e-6f)
            manifold.normal = dir / len;
        else
            manifold.normal = Vector2(0.0f, 1.0f);
    }
    
    manifold.penetration = shapeB.radius - distance;
    manifold.contactPoint = closest;
    
    return true;
}

// Main collision checking function
void Collision::CheckCollision(RigidBody &a, RigidBody &b)
{
    ShapeType typeA = a.collider ? a.collider->shape->GetType() : ShapeType::AABB;
    ShapeType typeB = b.collider ? b.collider->shape->GetType() : ShapeType::AABB;

    if (typeA == ShapeType::AABB && typeB == ShapeType::AABB)
    {   
        CollisionManifold m;
        // Use OBB collision for rotated boxes
        if (OBBvsOBB(a, b, m))
        {
            CollisionResolver::Resolve(a, b, m);
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
        // Use OBB collision for rotated boxes
        if (OBBvsCircle(b, a, *box, *circle, m))
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
        // Use OBB collision for rotated boxes
        if (OBBvsCircle(a, b, *box, *circle, m))
            CollisionResolver::Resolve(a, b, m);
    }
}