#pragma once

#include "random.h"

struct Room
{
    int width{};
    int height{};

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
};

class Generator
{
    Config config{};

public:
    explicit Generator(const Config config = Config())
        : config(config) {}

    Room Room() const;
};

inline Room Generator::Room() const
{
    int width = RandomInt(config.minWidth, config.maxWidth);
    int height = RandomInt(config.minHeight, config.maxHeight);

    return {width, height};
}
