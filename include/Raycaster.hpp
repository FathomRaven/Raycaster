#pragma once

#include "Graphics.hpp"
#include "Timer.hpp"
#include "Input.hpp"
#include "Math.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <istream>

struct Level
{
    unsigned int width, height;
    std::vector<std::string> data;
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

    Vector2 pos;
    Vector2 dir = {-1, 0};
    Vector2 plane = {0, 0.66};

    SDL_Rect ceiling;
    SDL_Rect floor;

    Level level;

    void LoadMap(const char* s);
};