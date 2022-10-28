#include "Raycaster.hpp"

void Raycaster::LoadMap(Level &level, const char* s) {
    
    std::vector<int> line;

	std::fstream fin (s);
    std::string output;
    while(getline(fin, output))
    {
        for (unsigned long x = 0; x < output.size(); x++)
        {
            if(output[x] != ',')
                line.push_back(std::atoi(&output[x]));    

        }
        // std::cout << "\n";
        level.data.push_back(line);
        line = {};
    }

    level.height = level.data.size();
    level.width = level.data[0].size();

	fin.close();
}

Raycaster::Raycaster()
{
    mGraphics = Graphics::Instance();
    mInput = Input::Instance();
    mTimer = Timer::Instance();

    LoadMap(level, "res/maps/level1.data");

    ceiling.x = 0;
    ceiling.y = 0;
    ceiling.w = mGraphics->SCREEN_WIDTH;
    ceiling.h = mGraphics->SCREEN_HEIGHT/2;

    floor.x = 0;
    floor.y = mGraphics->SCREEN_HEIGHT/2;
    floor.w = mGraphics->SCREEN_WIDTH;
    floor.h = mGraphics->SCREEN_HEIGHT/2;

    pos = {3.0f, 3.0f};

    for (unsigned int i = 0; i < level.height; i++)
    {
        for (unsigned int j = 0; j < level.width; j++)
        {
            std::cout << level.data[i][j];
        }
        std::cout << std::endl;
    }
    
}

Raycaster::~Raycaster()
{
    mGraphics = nullptr;
    mInput = nullptr;
    mTimer = nullptr;
}

void Raycaster::Render()
{
    mGraphics->DrawRectangle({130, 130, 130, 255}, ceiling);
    mGraphics->DrawRectangle({130, 70, 30, 255}, floor);

    //Draw here
    for (int x = 0; x < mGraphics->SCREEN_WIDTH; x++)
    {
        float cameraX = 2*x / float(mGraphics->SCREEN_WIDTH) - 1; //x-coord in camera space. "Normalizes" the coordinates so to speak
        //Ray position and direction
        Vector2 rayPos = {pos.x, pos.y};
        Vector2 rayDir = {dir.x + plane.x * cameraX, dir.y + plane.y * cameraX};
        Vector2 mapc = {(float)int(rayPos.x), (float)int(rayPos.y)}; //The current square of the map that the player is in
        Vector2 sideDist; //Distance the ray has to travel from starting position to the first x side and y side
        //Distance the ray has to travel from one side to the next, x or y. Simplified defintion thanks to lodev.org
        Vector2 deltaDist = {abs(1 / rayDir.x), abs(1 / rayDir.y)};
        float perpWallDist; //Length of ray
        Vector2 step; //Stores if the array goes in the positive direction or the negative direction
        int hit=0; //Was there a wall hit?
        int side; //What side was hit, North-South or East-West

        //Calculate step and initial side distance
        if(rayDir.x < 0)
        {
            step.x = -1;
            sideDist.x = (rayPos.x - mapc.x) * deltaDist.x;
        }
        else
        {
            step.x = 1;
            sideDist.x = (mapc.x + 1.0 - rayPos.x) * deltaDist.x;
        }

        if(rayDir.y < 0)
        {
            step.y = -1;
            sideDist.y = (rayPos.y - mapc.y) * deltaDist.y;
        }
        else
        {
            step.y = 1;
            sideDist.y = (mapc.y + 1.0 - rayPos.y) * deltaDist.y;
        }

        // check if the casted ray has hit an object
        while(hit == 0) // calculate ray until it hits an object
        {
            if(sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapc.x += step.x;
                side = 0;
            }
            else
            {
                sideDist.y += deltaDist.y;
                mapc.y += step.y;
                side = 1;
            }

            if(level.data[mapc.x][mapc.y]>0) // the ray has hit a non-zero block at position mapc.x, mapc.y
            {
                hit = 1;
            }
        }
        
        //Avoids fisheye effect
        if(side == 0) perpWallDist = (sideDist.x - deltaDist.x);
        else          perpWallDist = (sideDist.y - deltaDist.y);

        //Calculate the actual line height, and use that to generate the points to draw at
        int lineHeight = int(mGraphics->SCREEN_HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + mGraphics->SCREEN_HEIGHT / 2;
        if(drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + mGraphics->SCREEN_HEIGHT / 2;
        if(drawEnd >= mGraphics->SCREEN_HEIGHT) drawEnd = mGraphics->SCREEN_HEIGHT - 1;

        SDL_Color color;
        color.r = 0;
        color.g = 0;
        color.b = 0;
        color.a = 255;
        switch (level.data[mapc.x][mapc.y])
        {
        case 1:
            color.r = 255;
            break;
        case 2:
            color.g = 255;
            break;

        case 3:
            color.b = 255;
            break;

        case 4:
            color.r = 140;
            color.g = 70;
            color.b = 20;
            break;
        case 5:
            color.r = 255;
            color.g = 0;
            color.b = 255;
            break;
        
        default:
            color.g = 100;
            color.b = 100;
            break;
        }

        if(side == 1)
        {
            color.r = color.r / 2;
            color.g = color.g / 2;
            color.b = color.b / 2;
        }

        mGraphics->DrawLine(color, x, drawStart, x, drawEnd);
    }
}

void Raycaster::Update()
{
    double moveSpeed = mTimer->DeltaTime() * 5.0;
    double rotationSpeed = mTimer->DeltaTime() * 2.0;
    double angle = 1.571; // angle for side movement (-left/+right)

    if(mInput->KeyDown(SDL_SCANCODE_LSHIFT))
    {
        moveSpeed = mTimer->DeltaTime() * 8.0;
    }

    if (mInput->KeyDown(SDL_SCANCODE_W)) // forward movement with 'W' key
    {
        if(level.data[int(pos.x + dir.x * moveSpeed)][int(pos.y)] == 0) pos.x += dir.x * moveSpeed;
        if(level.data[int(pos.x)][int(pos.y + dir.y * moveSpeed)] == 0) pos.y += dir.y * moveSpeed;
    }

    if (mInput->KeyDown(SDL_SCANCODE_S)) // backward movement with S key
    {
        if(level.data[int(pos.x - dir.x * moveSpeed)][int(pos.y)] == 0) pos.x -= dir.x * moveSpeed;
        if(level.data[int(pos.x)][int(pos.y - dir.y * moveSpeed)] == 0) pos.y -= dir.y * moveSpeed;
    }

    if (mInput->KeyDown(SDL_SCANCODE_A)) // left movement with J key
    {
        double newDirX = dir.x * cos(angle) - dir.y * sin(angle);
        double newDirY = dir.x * sin(angle) + dir.y * cos(angle);

        if(level.data[int(pos.x + newDirX * moveSpeed)][int(pos.y)] == 0) pos.x += newDirX * moveSpeed;
        if(level.data[int(pos.x)][int(pos.y + newDirY * moveSpeed)] == 0) pos.y += newDirY * moveSpeed;
    }

    if (mInput->KeyDown(SDL_SCANCODE_D)) // right movement with L key
    {
        double newDirX = dir.x * cos(-angle) - dir.y * sin(-angle);
        double newDirY = dir.x * sin(-angle) + dir.y * cos(-angle);

        if(level.data[int(pos.x + newDirX * moveSpeed)][int(pos.y)] == 0) pos.x += newDirX * moveSpeed;
        if(level.data[int(pos.x)][int(pos.y + newDirY * moveSpeed)] == 0) pos.y += newDirY * moveSpeed;
    }

    if (mInput->KeyDown(SDL_SCANCODE_RIGHT)) // rotate to the right with D key
    {
        double oldDirX = dir.x;
        dir.x = dir.x * cos(-rotationSpeed) - dir.y * sin(-rotationSpeed);
        dir.y = oldDirX * sin(-rotationSpeed) + dir.y * cos(-rotationSpeed);

        double oldPlaneX = plane.x;
        plane.x = plane.x * cos(-rotationSpeed) - plane.y * sin(-rotationSpeed);
        plane.y = oldPlaneX * sin(-rotationSpeed) + plane.y * cos(-rotationSpeed);
    }

    if (mInput->KeyDown(SDL_SCANCODE_LEFT)) // rotate to the left with A key
    {
        double oldDirX = dir.x;
        dir.x = dir.x * cos(rotationSpeed) - dir.y * sin(rotationSpeed);
        dir.y = oldDirX * sin(rotationSpeed) + dir.y * cos(rotationSpeed);

        double oldPlaneX = plane.x;
        plane.x = plane.x * cos(rotationSpeed) - plane.y * sin(rotationSpeed);
        plane.y = oldPlaneX * sin(rotationSpeed) + plane.y * cos(rotationSpeed);
    }
}