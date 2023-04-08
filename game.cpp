// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
#include "game.h"
#include "dungeon.h"
#include "polygon.h"
#include "glm/glm.hpp"

bool roomsCreated = false;
bool roomsSeparated = false;
bool mainRoomsDecided = false;
bool createdConcavePolygon = false;
bool triangulatedPolygon = false;
glm::ivec2 circleCenter{512, 512};

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
    // Generate random color and invert that
    srand((unsigned int)time(nullptr));
    color = (rand() % 0xFFFFFF);
    invertedColor = color ^ 0xFFFFFF;
    
    renderState = RenderState::CREATE_ROOMS;
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick(const float deltaTime)
{
    // Render
    screen->Clear(0x000000);
    screen->Print((std::string("State: ") + to_string(renderState)).c_str(), 10, 10, 0xFFFFFF);

    if (roomsCreated)
    {
        for (const auto& room : rooms)
        {
            screen->Bar(room.position.x - room.halfSize.x, room.position.y - room.halfSize.y, room.position.x + room.halfSize.x, room.position.y + room.halfSize.y, color);
            screen->Box(room.position.x - room.halfSize.x, room.position.y - room.halfSize.y, room.position.x + room.halfSize.x, room.position.y + room.halfSize.y, 0xFFFFFF);
        }
    }

    if (mainRoomsDecided)
    {
        for (const auto& room : mainRooms)
        {
            screen->Bar(room.position.x - room.halfSize.x, room.position.y - room.halfSize.y, room.position.x + room.halfSize.x, room.position.y + room.halfSize.y, invertedColor);
            screen->Box(room.position.x - room.halfSize.x, room.position.y - room.halfSize.y, room.position.x + room.halfSize.x, room.position.y + room.halfSize.y, 0xFFFFFF);
        }
    }
    
    // Create rooms
    if (renderState == RenderState::CREATE_ROOMS && !roomsCreated)
    {
        rooms.clear();
        rooms.reserve(100);
        rooms = create_rooms(
            100,
            {12, 12},
            {72, 72},
            circleCenter,
            {150, 150},
            physics.GetWorld()
        );
        roomsCreated = true;

        renderState = RenderState::SEPARATE_ROOMS;
    }

    // Separate rooms
    if (renderState == RenderState::SEPARATE_ROOMS && !roomsSeparated)
    {
        // Sort rooms by distance from center of circle
        std::sort(rooms.begin(), rooms.end(), [](const Room& a, const Room& b) {
            return glm::length(static_cast<glm::vec2>(a.position) - static_cast<glm::vec2>(circleCenter)) < glm::length(static_cast<glm::vec2>(b.position) - static_cast<glm::vec2>(circleCenter));
        });
        
        // Collision resolution loop
        bool done = false;
        int iterations = 0;
        constexpr int maxIterations = 1000;
        while (!done && iterations < maxIterations) {
            done = true;
            for (unsigned int i = 0; i < rooms.size(); i++) {
                Room& a = rooms[i];
                for (unsigned int j = i + 1; j < rooms.size(); j++) {
                    Room& b = rooms[j];
                    if (check_overlap(a, b)) {
                        done = false;
                        resolve_overlap(a, b);
                    }
                }
            }
            iterations++;
        }
        
        roomsSeparated = true;
        renderState = RenderState::DECIDING_MAIN_ROOMS;
    }
    
    // Decide main rooms
    if (renderState == RenderState::DECIDING_MAIN_ROOMS && !mainRoomsDecided)
    {
        decide_main_rooms(rooms);
    
        for (const auto& room : rooms)
        {
            room.main
                ? mainRooms.push_back(room)
                : otherRooms.push_back(room);
        }

        mainRoomsDecided = true;
        renderState = RenderState::CREATE_CONCAVE_POLYGON;
    }
    
    // Create concave polygon
    if (renderState == RenderState::CREATE_CONCAVE_POLYGON && !createdConcavePolygon)
    {
        create_concave_simple_polygon(mainRooms, {512, 512});
        createdConcavePolygon = true;
        renderState = RenderState::TRIANGULATE;
    }
    else if (createdConcavePolygon)
    {
        for (unsigned int i = 0; i < mainRooms.size(); i++) {
            const auto current = mainRooms[i];
            const auto next = mainRooms[(i + 1) % mainRooms.size()];
            screen->Line(current.position.x, current.position.y, next.position.x, next.position.y, 0xFF0000);

            screen->Print(to_string(i).c_str(), current.position.x, current.position.y, 0x000000);
        }
    }
    
    // Triangulate
    if (renderState == RenderState::TRIANGULATE && !triangulatedPolygon)
    {
        std::vector<Room> polygon = mainRooms;
        while (polygon.size() > 3)
        {
            
        }
        
        triangulatedPolygon = true;
        renderState = RenderState::DONE;
    }
    // else if (triangulatedPolygon)
    // {
    //     for (unsigned int i = 0; i < triangles.size(); i += 3)
    //     {
    //         const auto p1 = triangles[i + 0];
    //         const auto p2 = triangles[i + 1];
    //         const auto p3 = triangles[i + 2];
    //
    //         screen->Line(p1.position.x, p1.position.y, p2.position.x, p2.position.y, 0x00FF00);
    //         screen->Line(p2.position.x, p2.position.y, p3.position.x, p3.position.y, 0x00FF00);
    //         screen->Line(p3.position.x, p3.position.y, p1.position.x, p1.position.y, 0x00FF00);
    //     }
    // }
}
