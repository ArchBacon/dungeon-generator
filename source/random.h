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

glm::ivec2 RandomPointInCirce(const float radius)
{
    const auto t = 2 * PI * RandomFloat();
    const auto u = RandomFloat() + RandomFloat();
    const auto r = u > 1 ? 2 - u : u;

    return {radius * r * cos(t), radius * r * sin(t)};
}

unsigned int randomHexColor()
{
    srand(time(NULL));
    return (rand() % 0xffffff);
}
