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
    const glm::ivec2 screenCenter = {SCRWIDTH / 2, SCRHEIGHT / 2};

    Config config{12, 72, 12, 72};
    const auto generator = Generator(config);
    Room rooms[100] = {};
    const auto color = randomHexColor();

    for (auto& room : rooms)
    {
        room = generator.Room();
        const auto point = RandomPointInCirce(150) + screenCenter;

        // Draw room with colored background and white border
        screen->Bar(point.x, point.y, point.x + room.width, point.y + room.height, color);
        screen->Box(point.x, point.y, point.x + room.width, point.y + room.height, 0xFFFFFF);
    }
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick(float deltaTime)
{
}
