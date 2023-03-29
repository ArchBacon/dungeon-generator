// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
#include "game.h"

#include "dungeon.h"

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
    screen->Clear(0);
    glm::ivec2 screenCenter = {SCRWIDTH / 2, SCRHEIGHT / 2};

    const auto generator = Generator();
    Room rooms[100] = {};

    for (auto& room : rooms)
    {
        room = generator.Room();
        auto rpic = RandomPointInCirce(80) + screenCenter;
        
        screen->Bar(
            rpic.x, rpic.y,
            rpic.x + room.width, rpic.y + room.height,
            randomHexColor()
        );

        screen->Box(
            rpic.x, rpic.y,
            rpic.x + room.width, rpic.y + room.height,
            0xFFFFFF
        );
    }
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick(float deltaTime)
{
}
