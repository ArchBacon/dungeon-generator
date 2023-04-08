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
    bool main = false;
};

Room create_room();
void create_position_for_room(Room& room, glm::ivec2 position, glm::ivec2 radius);
void decide_main_rooms(std::vector<Room>& rooms);
std::vector<Room> create_rooms(int amount, glm::ivec2 minSize, glm::ivec2 maxSize, glm::ivec2 position, glm::ivec2 radius, b2World* world);

glm::ivec2 random_point_in_circle(float radius);
glm::ivec2 random_point_in_ellipse(glm::vec2 radius);
float random_float(float min, float max);
int random_int(int min, int max);

bool check_overlap(const Room& a, const Room& b);
void resolve_overlap(Room& a, Room& b);

// -----------------------------------------------------------

inline Room create_room(const glm::ivec2 minSize, const glm::ivec2 maxSize)
{
    Room room{};
    room.halfSize = {
        random_int(minSize.x, maxSize.x),
        random_int(minSize.y, maxSize.y)
    };

    return room;
}

inline void create_position_for_room(Room& room, const glm::ivec2 position, const glm::ivec2 radius)
{
    room.position = position + random_point_in_ellipse(radius);
}

inline void decide_main_rooms(std::vector<Room>& rooms)
{
    glm::ivec2 mean{};
    for (const auto& room : rooms)
    {
        mean += room.halfSize;
    }
    mean /= static_cast<int>(rooms.size());

    for (auto& room : rooms)
    {
        if (room.halfSize.x > mean.x * 1.3 && room.halfSize.y > mean.y * 1.3)
        {
            room.main = true;
        }
    }
}

inline std::vector<Room> create_rooms(const int amount, const glm::ivec2 minSize, const glm::ivec2 maxSize, const glm::ivec2 position, const glm::ivec2 radius, b2World* world)
{
    std::vector<Room> rooms;
    rooms.reserve(amount);

    for (int i = 0; i < amount; i++)
    {
        auto room = create_room(minSize / 2, maxSize / 2);
        create_position_for_room(room, position, radius);

        rooms.push_back(room);
    }

    return rooms;
}

inline glm::ivec2 random_point_in_circle(const float radius)
{
    return random_point_in_ellipse({radius, radius});
}

inline glm::ivec2 random_point_in_ellipse(const glm::vec2 radius)
{
    const auto t = 2 * PI * random_float(0.0f, 1.0f);
    const auto u = random_float(0.0f, 1.0f) + random_float(0.0f, 1.0f);
    const auto r = u > 1 ? 2 - u : u;

    return {
        static_cast<int>(std::round(radius.x * r * cos(t))),
        static_cast<int>(std::round(radius.y * r * sin(t)))
    };
}

inline float random_float(const float min, const float max)
{
    static std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution distribution(min, max);
    return distribution(generator);
}

inline int random_int(const int min, const int max)
{
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution distribution(min, max);
    return distribution(generator);
}

// Check if two rooms overlap
inline bool check_overlap(const Room& a, const Room& b)
{
    const glm::ivec2 amin = a.position - a.halfSize;
    const glm::ivec2 amax = a.position + a.halfSize;
    const glm::ivec2 bmin = b.position - b.halfSize;
    const glm::ivec2 bmax = b.position + b.halfSize;
    return amin.x < bmax.x && amax.x > bmin.x && amin.y < bmax.y && amax.y > bmin.y;
}

// Resolve overlap between two rooms
inline void resolve_overlap(Room& a, Room& b)
{
    const glm::ivec2 amin = a.position - a.halfSize;
    const glm::ivec2 amax = a.position + a.halfSize;
    const glm::ivec2 bmin = b.position - b.halfSize;
    const glm::ivec2 bmax = b.position + b.halfSize;

    // Calculate overlap amounts in both axes
    const int xOverlap = std::min(amax.x, bmax.x) - std::max(amin.x, bmin.x);
    const int yOverlap = std::min(amax.y, bmax.y) - std::max(amin.y, bmin.y);

    // Move the room along its shortest axis
    if (xOverlap < yOverlap)
    {
        const int moveX = (a.position.x < b.position.x ? -1 : 1) * xOverlap / 2;
        a.position.x += moveX;
        b.position.x -= moveX;
    }
    else
    {
        const int moveY = (a.position.y < b.position.y ? -1 : 1) * yOverlap / 2;
        a.position.y += moveY;
        b.position.y -= moveY;
    }
}
