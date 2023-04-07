#pragma once

#include <vec2.hpp>
#include "dungeon.h"

bool ccw(glm::ivec2 a, glm::ivec2 b, glm::ivec2 c);
bool cw(glm::ivec2 a, glm::ivec2 b, glm::ivec2 c);
bool collinear(glm::ivec2 a, glm::ivec2 b, glm::ivec2 c);
int direction(glm::ivec2 a, glm::ivec2 b, glm::ivec2 c);
bool point_in_triangle(glm::ivec2 a, glm::ivec2 b, glm::ivec2 c, glm::ivec2 p);
void create_concave_simple_polygon(std::vector<glm::ivec2>& vertices, glm::ivec2 center);
std::vector<glm::ivec2> triangulate(const std::vector<glm::ivec2>& vertices);
bool is_ear(glm::ivec2 a, glm::ivec2 b, glm::ivec2 c, const std::vector<glm::ivec2>& vertices);

// -------------------- EXTENSION ----------------------------
void create_concave_simple_polygon(std::vector<Room>& rooms, glm::ivec2 center);
std::vector<Room> triangulate(const std::vector<Room>& rooms);
// -----------------------------------------------------------

bool ccw(const glm::ivec2 a, const glm::ivec2 b, const glm::ivec2 c)
{
    return direction(a, b, c) > 0;
}

bool cw(const glm::ivec2 a, const glm::ivec2 b, const glm::ivec2 c)
{
    return direction(a, b, c) < 0;
}

bool collinear(const glm::ivec2 a, const glm::ivec2 b, const glm::ivec2 c)
{
    return direction(a, b, c) == 0;
}

int direction(const glm::ivec2 a, const glm::ivec2 b, const glm::ivec2 c)
{
    return (c.y - b.y) * (b.x - a.x) - (b.y - a.y) * (c.x - b.x);
}

bool point_in_triangle(const glm::ivec2 a, const glm::ivec2 b, const glm::ivec2 c, const glm::ivec2 p)
{
    int area1 = (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
    int area2 = (p.x - b.x) * (c.y - b.y) - (p.y - b.y) * (c.x - b.x);
    int area3 = (p.x - c.x) * (a.y - c.y) - (p.y - c.y) * (a.x - c.x);

    const bool isClockwise = cw(a, b, c);
    if (!isClockwise)
    {
        area1 = -area1;
        area2 = -area2;
        area3 = -area3;
    }
    
    return (area1 >= 0 && area2 >= 0 && area3 >= 0) || (area1 <= 0 && area2 <= 0 && area3 <= 0);
}

void create_concave_simple_polygon(std::vector<glm::ivec2>& vertices, glm::ivec2 center)
{
    std::sort(vertices.begin(), vertices.end(), [center](const glm::ivec2 a, const glm::ivec2 b) {
        const float angleA = atan2(a.y - center.y, a.x - center.x);
        const float angleB = atan2(b.y - center.y, b.x - center.x);
        return angleA < angleB;
    });
}

inline std::vector<glm::ivec2> triangulate(const std::vector<glm::ivec2>& vertices)
{
    std::vector<glm::ivec2> triangles;
    std::vector<glm::ivec2> polygon = vertices;

    while (polygon.size() > 3)
    {
        for (int i = 0; i < polygon.size(); i++)
        {
            int prev = (i == 0) ? polygon.size() - 1 : i - 1;
            int next = (i == polygon.size() - 1) ? 0 : i + 1;

            if (is_ear(polygon[prev], polygon[i], polygon[next], polygon))
            {
                triangles.push_back(polygon[prev]);
                triangles.push_back(polygon[i]);
                triangles.push_back(polygon[next]);

                polygon.erase(polygon.begin() + i);
                i--;
            }
        }
    }

    triangles.push_back(polygon[0]);
    triangles.push_back(polygon[1]);
    triangles.push_back(polygon[2]);

    return triangles;
}

inline bool is_ear(const glm::ivec2 a, const glm::ivec2 b, const glm::ivec2 c, const std::vector<glm::ivec2>& vertices)
{
    // Check if the triangle is convex
    if (!ccw(a, b, c))
    {
        return false;
    }

    // Check if the triangle contains any other vertices
    for (const glm::ivec2& vertex : vertices)
    {
        if (vertex == a || vertex == b || vertex == c)
        {
            continue;
        }

        if (point_in_triangle(a, b, c, vertex))
        {
            return false;
        }
    }

    return true;
}

inline void create_concave_simple_polygon(std::vector<Room>& rooms, glm::ivec2 center)
{
    std::vector<glm::ivec2> vertices{};
    vertices.reserve(rooms.size());
    for (const Room& room : rooms)
    {
        vertices.push_back(room.position);
    }
    
    create_concave_simple_polygon(vertices, center);

    auto sortedRooms = std::vector<Room>{};
    for (const auto& position : vertices)
    {
        for (const auto& room : rooms)
        {
            if (room.position == position)
            {
                sortedRooms.emplace_back(room);
            }
        }
    }

    rooms = sortedRooms;
}

inline std::vector<Room> triangulate(const std::vector<Room>& rooms)
{
    std::vector<glm::ivec2> vertices{};
    vertices.reserve(rooms.size());
    for (const Room& room : rooms)
    {
        vertices.push_back(room.position);
    }

    auto triangles = triangulate(vertices);

    auto triangulatedRooms = std::vector<Room>{};
    for (const auto& position : vertices)
    {
        for (const auto& room : rooms)
        {
            if (room.position == position)
            {
                triangulatedRooms.emplace_back(room);
            }
        }
    }

    return triangulatedRooms;
}
