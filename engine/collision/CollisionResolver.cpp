#include "CollisionResolver.h"
#include <cmath>
#include <algorithm>
#include "../shapes/AABBShape.h"

void CollisionResolver::ResolveVelocity(const CollisionManifold &m)
{
    RigidBody* a = m.bodyA;
    RigidBody* b = m.bodyB;
    
    if (!a || !b) return;

    if (!a->isSleeping || !b->isSleeping) {
        a->isSleeping = false;
        b->isSleeping = false;
    }

    float totalInvMass = a->inverseMass + b->inverseMass;
    if (totalInvMass == 0.0f)
        return;

    for (int i = 0; i < m.contactCount; i++) {
        Vector2 ra = m.contactPoints[i] - a->position;
        Vector2 rb = m.contactPoints[i] - b->position;
        
        Vector2 va = a->velocity + Vector2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
        Vector2 vb = b->velocity + Vector2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);
        Vector2 rv = vb - va;
        
        float velAlongNormal = rv.dot(m.normal);

        // Objects are separating
        if (velAlongNormal > 0.0f)
            continue;

        // Use min for more physically realistic restitution combining
        float restitution = std::min(a->collider->restitution, b->collider->restitution);

        // Calculate impulse with angular components
        float raCrossN = ra.cross(m.normal);
        float rbCrossN = rb.cross(m.normal);
        float invMassSum = totalInvMass + raCrossN * raCrossN * a->inverseInertia + rbCrossN * rbCrossN * b->inverseInertia;
        
        float j = -(1.0f + restitution) * velAlongNormal;
        j /= invMassSum;
        j /= (float)m.contactCount; // Split impulse across contact points

        // ---- impulse resolution ----
        Vector2 impulse = m.normal * j;
        a->velocity -= impulse * a->inverseMass;
        b->velocity += impulse * b->inverseMass;

        // Angular velocity update
        a->angularVelocity -= ra.cross(impulse) * a->inverseInertia;
        b->angularVelocity += rb.cross(impulse) * b->inverseInertia;

        // ---- friction resolution ----
        // Recalculate relative velocity after normal impulse
        va = a->velocity + Vector2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
        vb = b->velocity + Vector2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);
        rv = vb - va;
        
        Vector2 tangent = rv - m.normal * rv.dot(m.normal);
        float tangentLen = tangent.length();

        if (tangentLen > 1e-6f)
        {
            tangent /= tangentLen;

            // Calculate friction impulse with angular components
            float raCrossT = ra.cross(tangent);
            float rbCrossT = rb.cross(tangent);
            float invMassSumFriction = totalInvMass + raCrossT * raCrossT * a->inverseInertia + rbCrossT * rbCrossT * b->inverseInertia;
            
            float jt = -rv.dot(tangent);
            jt /= invMassSumFriction;
            jt /= (float)m.contactCount; // Split friction impulse

            // Two-phase Coulomb friction model
            float muStatic = std::sqrt(a->collider->staticFriction * b->collider->staticFriction);
            float muDynamic = std::sqrt(a->collider->dynamicFriction * b->collider->dynamicFriction);

            Vector2 frictionImpulse;
            if (std::abs(jt) < muStatic * std::abs(j))
            {
                // Static friction: full tangent impulse (objects grip)
                frictionImpulse = tangent * jt;
            }
            else
            {
                // Dynamic friction: clamp to Coulomb limit (objects slide)
                float maxFriction = muDynamic * std::abs(j);
                jt = std::clamp(jt, -maxFriction, maxFriction);
                frictionImpulse = tangent * jt;
            }

            a->velocity -= frictionImpulse * a->inverseMass;
            b->velocity += frictionImpulse * b->inverseMass;
            
            // Apply angular friction
            a->angularVelocity -= ra.cross(frictionImpulse) * a->inverseInertia;
            b->angularVelocity += rb.cross(frictionImpulse) * b->inverseInertia;
        }
    }

    const float velocityEpsilon = 0.05f;
    const float angularEpsilon = 0.01f;

    if (std::abs(a->velocity.x) < velocityEpsilon)
        a->velocity.x = 0.0f;
    if (std::abs(a->velocity.y) < velocityEpsilon)
        a->velocity.y = 0.0f;
    if (std::abs(b->velocity.x) < velocityEpsilon)
        b->velocity.x = 0.0f;
    if (std::abs(b->velocity.y) < velocityEpsilon)
        b->velocity.y = 0.0f;
    
    // Clamp small angular velocities
    if (std::abs(a->angularVelocity) < angularEpsilon)
        a->angularVelocity = 0.0f;
    if (std::abs(b->angularVelocity) < angularEpsilon)
        b->angularVelocity = 0.0f;
}

void CollisionResolver::ResolvePosition(const CollisionManifold &m)
{
    RigidBody* a = m.bodyA;
    RigidBody* b = m.bodyB;
    
    if (!a || !b) return;

    float totalInvMass = a->inverseMass + b->inverseMass;
    if (totalInvMass == 0.0f)
        return;

    const float slop = 0.01f;   // allowed penetration
    const float percent = 0.15f; // correction strength

    float correctionMag = std::max(m.penetration - slop, 0.0f) / totalInvMass * percent;
    correctionMag /= (float)m.contactCount; // Split position correction

    for (int i = 0; i < m.contactCount; i++) {
        Vector2 correction = m.normal * correctionMag;
        a->position -= correction * a->inverseMass;
        b->position += correction * b->inverseMass;
    }
}
