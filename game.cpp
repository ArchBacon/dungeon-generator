// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
#include "game.h"
#include "dungeon.h"
#include "physics.h"
#include "box2d/box2d.h"

constexpr int NUM_ROOMS = 100;
int NUM_MAIN_ROOMS = 0;
constexpr glm::ivec2 SCREEN_CENTER = {512, 512};
int drawnRooms = 0;
int drawnMainRooms = 0;
Config config{12, 96, 12, 96};
Generator generator = Generator(config);
unsigned int color = randomHexColor();
unsigned int invertedColor = color ^ 0xFFFFFF;
Timer drawTimer;
Timer drawTimer2;
std::vector<Room> rooms{};
std::vector<int> mainRooms{};
glm::ivec2 mean{};
Timer physicsTimer{};
bool physicsDone = false;

void RedrawMainRooms(Surface* canvas, const Room& room)
{
    // Draw room with colored background and white border
    canvas->Bar(room.position.x, room.position.y, room.position.x + room.width, room.position.y + room.height, invertedColor);
    canvas->Box(room.position.x, room.position.y, room.position.x + room.width, room.position.y + room.height, 0xFFFFFF);
}

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
    rooms.resize(NUM_ROOMS);
    int totalWidth = 0;
    int totalHeight = 0;

    for (auto& room : rooms)
    {
        const auto point = RandomPointInCirce(150) + SCREEN_CENTER;
        room = generator.Room();
        room.position = point;
        
        totalWidth += room.width;
        totalHeight += room.height;

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set((float)room.position.x, (float)room.position.y);
        bodyDef.fixedRotation = true;
        b2Body* body = physics.GetWorld()->CreateBody(&bodyDef);

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox((float)room.width / 2, (float)room.height / 2);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        room.fixture = body->CreateFixture(&fixtureDef);
    }

    mean = {(float)totalWidth / NUM_ROOMS * 1.25f, (float)totalHeight / NUM_ROOMS * 1.25f};

    for (int i = 0; i < rooms.size(); i++)
    {
        Room& room = rooms[i];
        if (room.width >= mean.x && room.height >= mean.y)
        {
            NUM_MAIN_ROOMS++;
            mainRooms.emplace_back(i);
        }
    }
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick(const float deltaTime)
{
    screen->Clear(0x000000);

    for (int i = 0; i < drawnRooms; i++)
    {
        Room& room = rooms[i];
        DrawRoom(screen, room);
        screen->Print(to_string(i).c_str(), room.position.x + 10, room.position.y + 10, 0xFFFFFF);
    }

    if (drawTimer.elapsed() >= 0.02f && drawnRooms < NUM_ROOMS)
    {
        drawnRooms++;
        drawTimer.reset();
        physicsTimer.reset();
    }

    if (drawnRooms >= NUM_ROOMS)
    {
        physics.Update(deltaTime);
        physicsDone = true;
    }

    if (physicsTimer.elapsed() >= 1.0f && physicsDone)
    {
        std::vector<Room> mainRoomsList{};
        
        for (int i = 0; i < drawnMainRooms; i++)
        {
            Room& room = rooms[mainRooms[i]];
            mainRoomsList.emplace_back(room);
            RedrawMainRooms(screen, room);
        }

        std::sort(mainRoomsList.begin(), mainRoomsList.end(), [](const Room& a, const Room& b) {
            float angleA = atan2(a.position.y - 512, a.position.x - 512);
            float angleB = atan2(b.position.y - 512, b.position.x - 512);
            return angleA < angleB;
        });

        for (int i = 0; i < mainRoomsList.size(); i++)
        {
            Room& room = mainRoomsList[i];
            screen->Print(to_string(i).c_str(), room.position.x + 10, room.position.y + 10, 0x000000);
        }
    }
    
    if (drawTimer2.elapsed() >= 0.02f && drawnMainRooms < NUM_MAIN_ROOMS && physicsTimer.elapsed() >= 1.0f && physicsDone)
    {
        drawnMainRooms++;
        drawTimer2.reset();
    }

    for (Room& room : rooms)
    {
        room.position = {
            room.fixture->GetBody()->GetPosition().x - room.width / 2,
            room.fixture->GetBody()->GetPosition().y - room.height / 2
        };

        /**
        const auto body = room.fixture->GetBody();
        const b2Shape::Type shapeType = room.fixture->GetType();
        
        if (shapeType == b2Shape::e_polygon)
        {
            const auto* polygonShape = (b2PolygonShape*)room.fixture->GetShape();

            const b2Vec2 p1 = body->GetWorldPoint(polygonShape->m_vertices[0]);
            const b2Vec2 p2 = body->GetWorldPoint(polygonShape->m_vertices[1]);
            const b2Vec2 p3 = body->GetWorldPoint(polygonShape->m_vertices[2]);
            const b2Vec2 p4 = body->GetWorldPoint(polygonShape->m_vertices[3]);

            screen->Line(p1.x, p1.y, p2.x, p2.y, 0x0000FF);
            screen->Line(p2.x, p2.y, p3.x, p3.y, 0x0000FF);
            screen->Line(p3.x, p3.y, p4.x, p4.y, 0x0000FF);
            screen->Line(p4.x, p4.y, p1.x, p1.y, 0x0000FF);
        }
        */
    }
}

void Game::DrawRoom(Surface* canvas, const Room& room) const
{
    // Draw room with colored background and white border
    canvas->Bar(room.position.x, room.position.y, room.position.x + room.width, room.position.y + room.height, color);
    canvas->Box(room.position.x, room.position.y, room.position.x + room.width, room.position.y + room.height, 0xFFFFFF);
}
