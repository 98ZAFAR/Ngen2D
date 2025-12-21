#pragma once

class ForceGenerator{
    public:
    virtual ~ForceGenerator() = default;
    virtual void Apply(class RigidBody& body) = 0;
};