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

Physics::Physics()
{
    world = new b2World({0.0f, 0.0f});
    velocityIterations = 6;
    positionIterations = 2;
    updateRate = 60;
    timeStep = 1.0f / static_cast<float>(updateRate);
}

Physics::~Physics()
{
    delete world;
}

void Physics::Update(float deltaTime)
{
    for (int32 i = 0; i < updateRate; ++i)
    {
        world->Step(timeStep, velocityIterations, positionIterations);
    }
}