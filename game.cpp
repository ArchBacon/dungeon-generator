// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
#include "game.h"

#include "dungeon.h"

constexpr int NUM_ROOMS = 100;
constexpr glm::ivec2 SCREEN_CENTER = {512, 512};
int drawnRooms = 0;
Config config{12, 72, 12, 72};
Generator generator = Generator(config);
unsigned int color = randomHexColor();
Timer drawTimer;
std::vector<Room> rooms{};

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
    rooms.resize(NUM_ROOMS);
    
    for (auto& room : rooms)
    {
        const auto point = RandomPointInCirce(150) + SCREEN_CENTER;
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

    if (drawTimer.elapsed() >= 0.02f && drawnRooms < NUM_ROOMS)
    {
        drawnRooms++;
        drawTimer.reset();
    }
}

void Game::DrawRoom(Surface* canvas, const Room& room) const
{
    // Draw room with colored background and white border
    canvas->Bar(room.position.x, room.position.y, room.position.x + room.width, room.position.y + room.height, color);
    canvas->Box(room.position.x, room.position.y, room.position.x + room.width, room.position.y + room.height, 0xFFFFFF);
}
