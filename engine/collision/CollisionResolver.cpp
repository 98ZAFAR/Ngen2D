#include "CollisionResolver.h"
#include <cmath>

void CollisionResolver::Resolve(RigidBody& a, RigidBody& b)
{
    Vector2 delta = b.position - a.position;

    float overlapX = (a.size.x * 0.5f + b.size.x * 0.5f) - std::abs(delta.x);
    float overlapY = (a.size.y * 0.5f + b.size.y * 0.5f) - std::abs(delta.y);

    if (overlapX <= 0.0f || overlapY <= 0.0f)
        return;

    float totalInvMass = a.inverseMass + b.inverseMass;
    if (totalInvMass == 0.0f)
        return;

    // ---- collision normal ----
    Vector2 normal;
    float penetration;

    if (overlapX < overlapY) {
        normal = { (delta.x < 0) ? -1.0f : 1.0f, 0.0f };
        penetration = overlapX;
    } else {
        normal = { 0.0f, (delta.y < 0) ? -1.0f : 1.0f };
        penetration = overlapY;
    }

    // ---- positional correction ----
    const float percent = 0.8f;   // penetration correction strength
    Vector2 correction = normal * (penetration / totalInvMass * percent);

    a.position -= correction * a.inverseMass;
    b.position += correction * b.inverseMass;

    // ---- impulse resolution ----
    Vector2 relativeVelocity = b.velocity - a.velocity;
    float velAlongNormal = relativeVelocity.dot(normal);

    if (velAlongNormal > 0.0f)
        return;

    float e = a.restitution<b.restitution? a.restitution : b.restitution;

    float j = -(1.0f + e) * velAlongNormal;
    j /= totalInvMass;

    Vector2 impulse = normal * j;

    a.velocity -= impulse * a.inverseMass;
    b.velocity += impulse * b.inverseMass;
}