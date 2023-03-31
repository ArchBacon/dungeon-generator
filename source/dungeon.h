#pragma once

#include "random.h"

struct Room
{
    int width{};
    int height{};
    glm::ivec2 position{};
    glm::vec2 velocity{};

    Room() = default;
    Room(const int width, const int height)
        : width(width), height(height)
    {}
};

struct Config
{
    int minWidth = 10;
    int maxWidth = 50;
    int minHeight = 10;
    int maxHeight = 50;

    Config() = default;
    Config(const int minWidth, const int maxWidth, const int minHeight, const int maxHeight)
        : minWidth(minWidth), maxWidth(maxWidth), minHeight(minHeight), maxHeight(maxHeight)
    {}
};

class Generator
{
    Config& config;

public:
    explicit Generator(Config& config)
        : config(config)
    {}

    Room Room() const;
};

inline Room Generator::Room() const
{
    int width = RandomInt(config.minWidth, config.maxWidth);
    int height = RandomInt(config.minHeight, config.maxHeight);

    return {width, height};
}
