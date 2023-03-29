// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
#include "game.h"

#include "dungeon.h"

int roomToGenerate = 100;
int drawnRooms = 0;
Room rooms[100] = {};
const glm::ivec2 screenCenter = {SCRWIDTH / 2, SCRHEIGHT / 2};
Config config{12, 72, 12, 72};
const auto generator = Generator(config);
const auto color = randomHexColor();
Timer drawTimer;

void DrawRoom(Surface* canvas, const Room& room)
{
    // Draw room with colored background and white border
    canvas->Bar(room.position.x, room.position.y, room.position.x + room.width, room.position.y + room.height, color);
    canvas->Box(room.position.x, room.position.y, room.position.x + room.width, room.position.y + room.height, 0xFFFFFF);
}

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
    for (auto& room : rooms)
    {
        const auto point = RandomPointInCirce(150) + screenCenter;
        room = generator.Room();
        room.position = point;
    }
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick(float deltaTime)
{
    screen->Clear(0x000000);

    for (int i = 0; i < drawnRooms; i++)
    {
        DrawRoom(screen, rooms[i]);
    }

    if (drawTimer.elapsed() >= 0.02f && drawnRooms < roomToGenerate)
    {
        DrawRoom(screen, rooms[drawnRooms++]);
        drawTimer.reset();
    }
}
