#pragma once

#include "Math.hpp"

struct Sprite
{
    Vector2 position;
    float zPosition;
    int textureIndex;
    Vector2 scale = {1, 1};
};