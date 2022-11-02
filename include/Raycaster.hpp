#pragma once

#include "Graphics.hpp"
#include "Timer.hpp"
#include "Input.hpp"
#include "Math.hpp"

#include <vector>
#include <iostream>
#include <fstream>

struct Level
{
    unsigned int width, height;
    std::vector<std::vector<int>> data;
    Vector2 playerPosition;
};

class Raycaster
{
public:
    void Render();
    void Update();

    Raycaster();
    ~Raycaster();

private: 
    Timer* mTimer;
    Graphics* mGraphics;
    Input* mInput;

    void LoadMap(Level &map, std::string s);

    Vector2 pos;
    Vector2 dir = {-1, 0};
    Vector2 plane = {0, 0.66};

    Level level;
    unsigned int currentLevel = 1;
    unsigned int maxLevels = 2;
    std::string levelData;

    Texture textures[5];
    Texture floorTexture;
    Texture ceilingTexture;
    Texture defaultTexture;
};