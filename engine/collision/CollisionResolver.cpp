#include "CollisionResolver.h"
#include <cmath>
#include <algorithm>
#include "../shapes/AABBShape.h"

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

    // Recalculate contact vectors after position correction
    Vector2 ra = m.contactPoint - a.position;
    Vector2 rb = m.contactPoint - b.position;
    
    Vector2 va = a.velocity + Vector2(-a.angularVelocity * ra.y, a.angularVelocity * ra.x);
    Vector2 vb = b.velocity + Vector2(-b.angularVelocity * rb.y, b.angularVelocity * rb.x);
    Vector2 rv = vb - va;
    
    float velAlongNormal = rv.dot(m.normal);

    // Objects are separating
    if (velAlongNormal > 0.0f)
        return;

    float restitution = (a.collider->restitution + b.collider->restitution) / 2.0f;

    // Calculate impulse with angular components
    float raCrossN = ra.cross(m.normal);
    float rbCrossN = rb.cross(m.normal);
    float invMassSum = totalInvMass + raCrossN * raCrossN * a.inverseInertia + rbCrossN * rbCrossN * b.inverseInertia;
    
    float j = -(1.0f + restitution) * velAlongNormal;
    j /= invMassSum;

    // ---- impulse resolution ----
    Vector2 impulse = m.normal * j;
    a.velocity -= impulse * a.inverseMass;
    b.velocity += impulse * b.inverseMass;

    // Angular velocity update
    a.angularVelocity -= ra.cross(impulse) * a.inverseInertia;
    b.angularVelocity += rb.cross(impulse) * b.inverseInertia;

    // ---- friction resolution ----
    // Recalculate relative velocity after normal impulse
    va = a.velocity + Vector2(-a.angularVelocity * ra.y, a.angularVelocity * ra.x);
    vb = b.velocity + Vector2(-b.angularVelocity * rb.y, b.angularVelocity * rb.x);
    rv = vb - va;
    
    Vector2 tangent = rv - m.normal * rv.dot(m.normal);
    float tangentLen = tangent.length();

    if (tangentLen > 1e-6f)
    {
        tangent /= tangentLen;

        // Calculate friction impulse with angular components
        float raCrossT = ra.cross(tangent);
        float rbCrossT = rb.cross(tangent);
        float invMassSumFriction = totalInvMass + raCrossT * raCrossT * a.inverseInertia + rbCrossT * rbCrossT * b.inverseInertia;
        
        float jt = -rv.dot(tangent);
        jt /= invMassSumFriction;

        // Use geometric mean for friction coefficient
        float mu = std::sqrt(a.collider->dynamicFriction * b.collider->dynamicFriction);

        float maxFriction = mu * std::abs(j);

        jt = std::clamp(jt, -maxFriction, maxFriction);

        Vector2 frictionImpulse = tangent * jt;

        a.velocity -= frictionImpulse * a.inverseMass;
        b.velocity += frictionImpulse * b.inverseMass;
        
        // Apply angular friction
        a.angularVelocity -= ra.cross(frictionImpulse) * a.inverseInertia;
        b.angularVelocity += rb.cross(frictionImpulse) * b.inverseInertia;
    }

    const float velocityEpsilon = 0.5f;
    const float angularEpsilon = 0.05f; // Increased to stop spinning earlier

    if (std::abs(a.velocity.x) < velocityEpsilon)
        a.velocity.x = 0.0f;
    if (std::abs(a.velocity.y) < velocityEpsilon)
        a.velocity.y = 0.0f;
    if (std::abs(b.velocity.x) < velocityEpsilon)
        b.velocity.x = 0.0f;
    if (std::abs(b.velocity.y) < velocityEpsilon)
        b.velocity.y = 0.0f;
    
    // Clamp small angular velocities
    if (std::abs(a.angularVelocity) < angularEpsilon)
        a.angularVelocity = 0.0f;
    if (std::abs(b.angularVelocity) < angularEpsilon)
        b.angularVelocity = 0.0f;
}
