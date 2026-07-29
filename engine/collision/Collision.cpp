#include "Collision.h"
#include "CollisionResolver.h"
#include "../math/MathUtils.h"
#include <vector>

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
    AABBShape *shapeA = static_cast<AABBShape *>(a.collider->shape.get());
    AABBShape *shapeB = static_cast<AABBShape *>(b.collider->shape.get());
    
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
    bool axisFromA = false;
    
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
            axisFromA = (i < 2);
            
            // Make sure normal points from A to B
            Vector2 centerDiff = b.position - a.position;
            if (centerDiff.dot(smallestAxis) < 0)
                smallestAxis = smallestAxis * -1.0f;
        }
    }
    
    // Collision detected, fill manifold
    manifold.normal = smallestAxis;
    manifold.penetration = minOverlap;
    
    // Find contact point by finding the deepest vertex
    if (axisFromA)
    {
        // Axis from A, so B's vertex is hitting A's face.
        // Find deepest vertex of B along -normal (minimum dot product with normal)
        float minProj = FLT_MAX;
        std::vector<Vector2> supports;
        
        for (int i = 0; i < 4; i++)
        {
            float proj = cornersB[i].dot(manifold.normal);
            if (proj < minProj - 0.01f)
            {
                minProj = proj;
                supports.clear();
                supports.push_back(cornersB[i]);
            }
            else if (std::abs(proj - minProj) <= 0.01f)
            {
                supports.push_back(cornersB[i]);
            }
        }
        
        if (supports.size() == 1) {
            manifold.contactCount = 1;
            manifold.contactPoints[0] = supports[0];
        } else if (supports.size() >= 2) {
            manifold.contactCount = 2;
            manifold.contactPoints[0] = supports[0];
            manifold.contactPoints[1] = supports[1];
        }
    }
    else
    {
        // Axis from B, so A's vertex is hitting B's face.
        // Find deepest vertex of A along normal (maximum dot product with normal)
        float maxProj = -FLT_MAX;
        std::vector<Vector2> supports;
        
        for (int i = 0; i < 4; i++)
        {
            float proj = cornersA[i].dot(manifold.normal);
            if (proj > maxProj + 0.01f)
            {
                maxProj = proj;
                supports.clear();
                supports.push_back(cornersA[i]);
            }
            else if (std::abs(proj - maxProj) <= 0.01f)
            {
                supports.push_back(cornersA[i]);
            }
        }
        
        if (supports.size() == 1) {
            manifold.contactCount = 1;
            manifold.contactPoints[0] = supports[0];
        } else if (supports.size() >= 2) {
            manifold.contactCount = 2;
            manifold.contactPoints[0] = supports[0];
            manifold.contactPoints[1] = supports[1];
        }
    }
    
    return true;
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
    manifold.contactCount = 1;
    manifold.contactPoints[0] = a.position + manifold.normal * shapeA.radius;
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
    manifold.contactCount = 1;
    manifold.contactPoints[0] = closest;
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
    manifold.contactCount = 1;
    manifold.contactPoints[0] = closest;
    
    return true;
}

// Main collision checking function
bool Collision::CheckCollision(RigidBody &a, RigidBody &b, CollisionManifold& outManifold)
{
    ShapeType typeA = a.collider ? a.collider->shape->GetType() : ShapeType::AABB;
    ShapeType typeB = b.collider ? b.collider->shape->GetType() : ShapeType::AABB;

    bool hit = false;
    if (typeA == ShapeType::AABB && typeB == ShapeType::AABB)
    {   
        // Use OBB collision for rotated boxes
        hit = OBBvsOBB(a, b, outManifold);
    }
    else if (typeA == ShapeType::Circle && typeB == ShapeType::Circle)
    {
        auto *shapeA = static_cast<CircleShape *>(a.collider->shape.get());
        auto *shapeB = static_cast<CircleShape *>(b.collider->shape.get());

        hit = CirclevsCircle(a, b, *shapeA, *shapeB, outManifold);
    }
    else if (typeA == ShapeType::Circle && typeB == ShapeType::AABB)
    {
        auto *circle = static_cast<CircleShape *>(a.collider->shape.get());
        auto *box = static_cast<AABBShape *>(b.collider->shape.get());

        // Use OBB collision for rotated boxes
        if (OBBvsCircle(b, a, *box, *circle, outManifold))
        {
            outManifold.normal = outManifold.normal * -1.0f;
            hit = true;
        }
    }
    else if (typeA == ShapeType::AABB && typeB == ShapeType::Circle)
    {
        auto *box = static_cast<AABBShape *>(a.collider->shape.get());
        auto *circle = static_cast<CircleShape *>(b.collider->shape.get());

        // Use OBB collision for rotated boxes
        hit = OBBvsCircle(a, b, *box, *circle, outManifold);
    }

    if (hit)
    {
        outManifold.bodyA = &a;
        outManifold.bodyB = &b;
    }
    return hit;
}