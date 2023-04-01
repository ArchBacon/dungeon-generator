#pragma once
#include <box2d/b2_world.h>

class Physics
{
    b2World* world{};
    int updateRate{}; // Physics update rate
    float timeStep{};
    int32 velocityIterations{};
    int32 positionIterations{};
    
public:
    Physics();
    virtual ~Physics();

    b2World* GetWorld() const { return world; }
    void Update(float deltaTime);
};
