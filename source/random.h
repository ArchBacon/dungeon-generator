#pragma once

#include <random>
#include <glm/glm.hpp>

int RandomInt(const int min, const int max)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> dist(min, max); // define the range

    return dist(gen);
}

double roundm(double n, double m)
{
    return floor(((n + m - 1) / m) * m);
}

glm::ivec2 RandomPointInCirce(const float radius)
{
    const auto t = 2 * PI * RandomFloat();
    const auto u = RandomFloat() + RandomFloat();
    const auto r = u > 1 ? 2 - u : u;

    return {
        roundm(radius * r * cos(t), 4),
        roundm(radius * r * sin(t), 4)
    };
}

unsigned int randomHexColor()
{
    srand(time(NULL));
    return (rand() % 0xffffff);
}
