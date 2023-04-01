// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
#include "game.h"
#include "dungeon.h"
#include "physics.h"
#include "box2d/box2d.h"

constexpr int NUM_ROOMS = 100;
constexpr glm::ivec2 SCREEN_CENTER = {512, 512};
int drawnRooms = 0;
Config config{12, 72, 12, 72};
Generator generator = Generator(config);
unsigned int color = randomHexColor();
unsigned int invertedColor = color ^ 0xFFFFFF;
Timer drawTimer;
std::vector<Room> rooms{};
glm::ivec2 mean{};

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

        if (room.width >= mean.x && room.height >= mean.y)
        {
            RedrawMainRooms(screen, room);
        }
    }

    if (drawTimer.elapsed() >= 0.02f && drawnRooms < NUM_ROOMS)
    {
        drawnRooms++;
        drawTimer.reset();
    }

    if (drawnRooms >= NUM_ROOMS)
    {
        physics.Update(deltaTime);
    }

    for (Room& room : rooms)
    {
        room.position = {
            room.fixture->GetBody()->GetPosition().x - room.width / 2,
            room.fixture->GetBody()->GetPosition().y - room.height / 2
        };
        
        const auto body = room.fixture->GetBody();
        const b2Shape::Type shapeType = room.fixture->GetType();
        
        if (shapeType == b2Shape::e_polygon)
        {
            const auto* polygonShape = (b2PolygonShape*)room.fixture->GetShape();

            const b2Vec2 p1 = body->GetWorldPoint(polygonShape->m_vertices[0]);
            const b2Vec2 p2 = body->GetWorldPoint(polygonShape->m_vertices[1]);
            const b2Vec2 p3 = body->GetWorldPoint(polygonShape->m_vertices[2]);
            const b2Vec2 p4 = body->GetWorldPoint(polygonShape->m_vertices[3]);

            // screen->Line(p1.x, p1.y, p2.x, p2.y, 0x0000FF);
            // screen->Line(p2.x, p2.y, p3.x, p3.y, 0x0000FF);
            // screen->Line(p3.x, p3.y, p4.x, p4.y, 0x0000FF);
            // screen->Line(p4.x, p4.y, p1.x, p1.y, 0x0000FF);
        }
    }
}

void Game::DrawRoom(Surface* canvas, const Room& room) const
{
    // Draw room with colored background and white border
    canvas->Bar(room.position.x, room.position.y, room.position.x + room.width, room.position.y + room.height, color);
    canvas->Box(room.position.x, room.position.y, room.position.x + room.width, room.position.y + room.height, 0xFFFFFF);
}
