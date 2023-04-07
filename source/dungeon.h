#pragma once

#include <random>
#include <vec2.hpp>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_world.h>

struct Room
{
    glm::ivec2 halfSize{};
    glm::ivec2 position{};
    b2Fixture* fixture{};
    bool main = false;
};

Room create_room();
void create_position_for_room(Room& room, glm::ivec2 position, glm::ivec2 radius);
void create_fixture_for_room(b2World* world, Room& room);
void decide_main_rooms(std::vector<Room>& rooms);
std::vector<Room> create_rooms(int amount, glm::ivec2 minSize, glm::ivec2 maxSize, glm::ivec2 position, glm::ivec2 radius, b2World* world);

glm::ivec2 random_point_in_circle(float radius);
glm::ivec2 random_point_in_ellipse(glm::vec2 radius);
float random_float(float min, float max);
int random_int(int min, int max);

// -----------------------------------------------------------

Room create_room(const glm::ivec2 minSize, const glm::ivec2 maxSize)
{
    Room room{};
    room.halfSize = {
        random_int(minSize.x, maxSize.x),
        random_int(minSize.y, maxSize.y)
    };

    return room;
}

void create_position_for_room(Room& room, const glm::ivec2 position, const glm::ivec2 radius)
{
    room.position = position + random_point_in_ellipse(radius);
}

void create_fixture_for_room(b2World* world, Room& room)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(static_cast<float>(room.position.x), static_cast<float>(room.position.y));
    b2Body* body = world->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(static_cast<float>(room.halfSize.x), static_cast<float>(room.halfSize.y));

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    room.fixture = body->CreateFixture(&fixtureDef);
}

inline void decide_main_rooms(std::vector<Room>& rooms)
{
    glm::ivec2 mean{};
    for (const auto& room : rooms)
    {
        mean += room.position;
    }
    mean /= static_cast<int>(rooms.size());
    mean *= 1.25f;

    for (auto& room : rooms)    
    {
        if (room.position.x > mean.x && room.position.y > mean.y)
        {
            room.main = true;
        }
    }
}

std::vector<Room> create_rooms(const int amount, const glm::ivec2 minSize, const glm::ivec2 maxSize, const glm::ivec2 position, const glm::ivec2 radius, b2World* world)
{
    std::vector<Room> rooms;
    rooms.reserve(amount);

    for (int i = 0; i < amount; i++)
    {
        auto room = create_room(minSize / 2, maxSize / 2);
        create_position_for_room(room, position, radius);
        create_fixture_for_room(world, room);
        
        rooms.push_back(room);
    }
}

glm::ivec2 random_point_in_circle(const float radius)
{
    return random_point_in_ellipse({radius, radius});
}

glm::ivec2 random_point_in_ellipse(const glm::vec2 radius)
{
    const auto t = 2 * PI * random_float(0.0f, 1.0f);
    const auto u = random_float(0.0f, 1.0f) + random_float(0.0f, 1.0f);
    const auto r = u > 1 ? 2 - u : u;

    return {
        static_cast<int>(std::round(radius.x * r * cos(t))),
        static_cast<int>(std::round(radius.y * r * sin(t)))
    };
}

float random_float(const float min, const float max)
{
    static std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution distribution(min, max);
    return distribution(generator);
}

int random_int(const int min, const int max)
{
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution distribution(min, max);
    return distribution(generator);
}
