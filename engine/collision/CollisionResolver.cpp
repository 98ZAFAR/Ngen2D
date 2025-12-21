#include "CollisionResolver.h"
#include <cmath>
#include <algorithm>
#include "../shapes/AABBShape.h"

void CollisionResolver::Resolve(RigidBody &a, RigidBody &b)
{
    a.sleepTime = 0.0f;
    b.sleepTime = 0.0f;
    a.isSleeping = false;
    b.isSleeping = false;

    // ---- early out ----
    Vector2 delta = b.position - a.position;

    AABBShape *shapeA = static_cast<AABBShape *>(a.collider->shape);
    AABBShape *shapeB = static_cast<AABBShape *>(b.collider->shape);

    float overlapX = (shapeA->halfsize.x + shapeB->halfsize.x) - std::abs(delta.x);
    float overlapY = (shapeA->halfsize.y + shapeB->halfsize.y) - std::abs(delta.y);

    if (overlapX <= 0.0f || overlapY <= 0.0f)
        return;

    float totalInvMass = a.inverseMass + b.inverseMass;
    if (totalInvMass == 0.0f)
        return;

    // ---- collision normal ----
    Vector2 normal;
    float penetration;

    if (overlapX < overlapY)
    {
        normal = {(delta.x < 0) ? -1.0f : 1.0f, 0.0f};
        penetration = overlapX;
    }
    else
    {
        normal = {0.0f, (delta.y < 0) ? -1.0f : 1.0f};
        penetration = overlapY;
    }

    // ---- positional correction ----
    const float slop = 0.01f;   // penetration allowance
    const float percent = 0.8f; // penetration correction strength
    Vector2 correction = normal * (std::max(penetration - slop, 0.0f) / totalInvMass * percent);

    a.position -= correction * a.inverseMass;
    b.position += correction * b.inverseMass;

    // ---- impulse resolution ----
    Vector2 relativeVelocity = b.velocity - a.velocity;
    float velAlongNormal = relativeVelocity.dot(normal);

    if (velAlongNormal > 0.0f)
        return;

    // float e = std::min(a.collider->restitution, b.collider->restitution);
    float e = (a.collider->restitution + b.collider->restitution) / 2.0f;

    float j = -(1.0f + e) * velAlongNormal;
    j /= totalInvMass;

    Vector2 impulse = normal * j;

    a.velocity -= impulse * a.inverseMass;
    b.velocity += impulse * b.inverseMass;

    // ---- friction resolution ----
    // Calculate friction using original relative velocity (before normal impulse)
    Vector2 tangent = relativeVelocity - normal * velAlongNormal;
    float tangentLen = tangent.length();

    if (tangentLen > 1e-6f)
    {
        tangent /= tangentLen;

        float jt = -relativeVelocity.dot(tangent);
        jt /= totalInvMass;

        // Use geometric mean for friction coefficient
        float mu = std::sqrt(a.collider->dynamicFriction * b.collider->dynamicFriction);

        float maxFriction = mu * std::abs(j);

        jt = std::clamp(jt, -maxFriction, maxFriction);

        Vector2 frictionImpulse = tangent * jt;

        a.velocity -= frictionImpulse * a.inverseMass;
        b.velocity += frictionImpulse * b.inverseMass;
    }
}

void CollisionResolver::Resolve(
    RigidBody &a,
    RigidBody &b,
    const CollisionManifold &m)
{
    a.sleepTime = 0.0f;
    b.sleepTime = 0.0f;
    a.isSleeping = false;
    b.isSleeping = false;
    // ---- early out ----
    float totalInvMass = a.inverseMass + b.inverseMass;
    if (totalInvMass == 0.0f)
        return;

    const float slop = 0.01f;   // allowed penetration
    const float percent = 0.8f; // correction strength

    float correctionMag =
        std::max(m.penetration - slop, 0.0f) / totalInvMass * percent;

    Vector2 correction = m.normal * correctionMag;

    a.position -= correction * a.inverseMass;
    b.position += correction * b.inverseMass;

    Vector2 rv = b.velocity - a.velocity;
    float velAlongNormal = rv.dot(m.normal);

    // Objects are separating
    if (velAlongNormal > 0.0f)
        return;

    // float restitution = std::min(
    //     a.collider->restitution,
    //     b.collider->restitution
    // );

    float restitution = (a.collider->restitution + b.collider->restitution) / 2.0f;

    float j = -(1.0f + restitution) * velAlongNormal;
    j /= totalInvMass;

    Vector2 impulse = m.normal * j;
    a.velocity -= impulse * a.inverseMass;
    b.velocity += impulse * b.inverseMass;

    // ---- friction resolution ----
    // Calculate friction using original relative velocity (before normal impulse)
    Vector2 tangent = rv - m.normal * velAlongNormal;
    float tangentLen = tangent.length();

    if (tangentLen > 1e-6f)
    {
        tangent /= tangentLen;

        float jt = -rv.dot(tangent);
        jt /= totalInvMass;

        // Use geometric mean for friction coefficient
        float mu = std::sqrt(a.collider->dynamicFriction * b.collider->dynamicFriction);

        float maxFriction = mu * std::abs(j);

        jt = std::clamp(jt, -maxFriction, maxFriction);

        Vector2 frictionImpulse = tangent * jt;

        a.velocity -= frictionImpulse * a.inverseMass;
        b.velocity += frictionImpulse * b.inverseMass;
    }

    const float velocityEpsilon = 0.5f;

    if (std::abs(a.velocity.x) < velocityEpsilon)
        a.velocity.x = 0.0f;
    if (std::abs(a.velocity.y) < velocityEpsilon)
        a.velocity.y = 0.0f;
    if (std::abs(b.velocity.x) < velocityEpsilon)
        b.velocity.x = 0.0f;
    if (std::abs(b.velocity.y) < velocityEpsilon)
        b.velocity.y = 0.0f;
}
