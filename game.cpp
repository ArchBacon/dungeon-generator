// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
#include "game.h"
#include "dungeon.h"
#include "polygon.h"

bool roomsCreated = false;
bool roomsSeparated = false;
bool mainRoomsDecided = false;
bool createdConcavePolygon = false;
bool triangulatedPolygon = false;

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
    // Generate random color and invert that
    srand(time(nullptr));
    color = (rand() % 0xffffff);
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
    // TODO: Render rooms
    // TODO: Render Polygon
    // TODO: Render Triangles
    
    // Create rooms
    if (renderState == RenderState::CREATE_ROOMS && !roomsCreated)
    {
        rooms.clear();
        rooms.reserve(100);
        rooms = create_rooms(
            100,
            {12, 12},
            {72, 72},
            {512, 512},
            {150, 150},
            physics.GetWorld()
        );
        roomsCreated = true;
        
        renderState = RenderState::SEPARATE_ROOMS;
    }

    // Separate rooms
    if (renderState == RenderState::SEPARATE_ROOMS && !roomsSeparated)
    {
        physics.Update(deltaTime);

        bool allAsleep = true;
        for (auto& room : rooms)
        {
            const auto pos = room.fixture->GetBody()->GetPosition();
            room.position = {pos.x, pos.y};

            if (room.fixture->GetBody()->IsAwake())
            {
                allAsleep = false;
            }
        }

        if (allAsleep)
        {
            roomsSeparated = true;
            renderState = RenderState::DECIDING_MAIN_ROOMS;
        }
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

        renderState = RenderState::CREATE_CONCAVE_POLYGON;
    }

    // Create concave polygon
    if (renderState == RenderState::CREATE_CONCAVE_POLYGON && !createdConcavePolygon)
    {
        create_concave_simple_polygon(mainRooms, {512, 512});
        renderState = RenderState::TRIANGULATE;
    }

    // Triangulate
    if (renderState == RenderState::TRIANGULATE && !triangulatedPolygon)
    {
        const auto triangles = triangulate(mainRooms);
        renderState = RenderState::DONE;
    }
}
