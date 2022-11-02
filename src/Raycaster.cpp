/*
* TODO:
* Flip maps "x" and "y", to be inline with the vector2,
* Positions are weird and bugged; Possibly related to above issue. Might be a problem with map loader
* Something weird going on with non-square textures
* Transparency doesn't work (?)
*/

#include "Raycaster.hpp"

void Raycaster::LoadMap(Level &map, std::string s) {
    
    map = {};

    std::vector<int> line;

	std::fstream fin (s.c_str());
    std::string output;
    int y = 0;
    while(getline(fin, output))
    {
        for (unsigned long x = 0; x < output.size(); x++)
        {
            if(output[x] != ',')
                line.push_back(std::atoi(&output[x]));   

            if(output[x] == 'P')
            {
                // std::cout << y + 1<< ", " << x << "\n";
                map.playerPosition = {(float)y + 1, (float)x };   
            }
        }
        // std::cout << "\n";
        map.data.push_back(line);
        line = {};
        y++;
    }

    map.height = map.data.size();
    map.width = map.data[0].size();

	fin.close();
}

Raycaster::Raycaster()
{
    mGraphics = Graphics::Instance();
    mInput = Input::Instance();
    mTimer = Timer::Instance();

    levelData = "res/maps/level" + std::to_string(currentLevel) + ".data";
    LoadMap(level, levelData);

    pos = level.playerPosition;
    // pos = {3.0, 3.0};

    mGraphics->LoadTexture("res/textures/missing.png", defaultTexture);

    mGraphics->LoadTexture("res/textures/bricks.png", textures[0]);
    mGraphics->LoadTexture("res/textures/stonewall.png", textures[1]);
    mGraphics->LoadTexture("res/textures/bone_wall.png", textures[2]);
    mGraphics->LoadTexture("res/textures/bone_wall_pillar.png", textures[3]);
    mGraphics->LoadTexture("res/textures/cat.png", textures[4]);

    mGraphics->LoadTexture("res/textures/metal.png", floorTexture);
    mGraphics->LoadTexture("res/textures/metal_ceiling.png", ceilingTexture);

    // for (unsigned int i = 0; i < level.height; i++)
    // {
    //     for (unsigned int j = 0; j < level.width; j++)
    //     {
    //         std::cout << level.data[i][j];
    //     }
    //     std::cout << std::endl;
    // }
}

Raycaster::~Raycaster()
{
    mGraphics = nullptr;
    mInput = nullptr;
    mTimer = nullptr;
}

void Raycaster::Render()
{
    //Render floor and ceiling
    for(int y = 0; y < mGraphics->SCREEN_HEIGHT; y++)
    {
      // Leftmost rays
      Vector2 rayDir0(dir.x - plane.x, dir.y - plane.y);
      Vector2 rayDir1(dir.x + plane.x, dir.y + plane.y);

      // Y position compared to horizon
      int p = y - mGraphics->SCREEN_HEIGHT / 2;

      // Vertical position of the camera
      float posZ = 0.5 * mGraphics->SCREEN_HEIGHT;

      // Horizontal distance from the camera to the floor for the current row we're drawing
      // 0.5 is the z position exactly in the middle between the floor and ceiling
      float rowDistance = posZ / p;

      // Calculate the real world step vector we have to add for each x (parallel to camera plane)
      // Adding step by step avoids multiplications with a weight in the inner loop
      float floorStepX = rowDistance * (rayDir1.x - rayDir0.x) / mGraphics->SCREEN_WIDTH;
      float floorStepY = rowDistance * (rayDir1.y - rayDir0.y) / mGraphics->SCREEN_WIDTH;

      // Real world coordinates of the leftmost column. This will be updated as we step to the right.
      float floorX = pos.x + rowDistance * rayDir0.x;
      float floorY = pos.y + rowDistance * rayDir0.y;

      for(int x = 0; x < mGraphics->SCREEN_WIDTH; ++x)
      {
        // The cell coord is simply got from the integer parts of floorX and floorY
        int cellX = (int)(floorX);
        int cellY = (int)(floorY);

        // Get the texture coordinate from the fractional part
        int tx = (int)(floorTexture.width * (floorX - cellX)) & (floorTexture.width - 1);
        int ty = (int)(floorTexture.height * (floorY - cellY)) & (floorTexture.height - 1);

        floorX += floorStepX;
        floorY += floorStepY;

        //Color
        SDL_Color color;

        // Draw floor
        color = floorTexture.GetPixel(tx, ty);
        mGraphics->buffer[y][x] = color;

        //Draw ceiling (symmetrical, at screen height - y - 1 instead of y)
        color = ceilingTexture.GetPixel(tx, ty);
        mGraphics->buffer[mGraphics->SCREEN_HEIGHT - y - 1][x] = color;
      }
    }

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

        int texNum = level.data[mapc.x][mapc.y] - 1; //1 subtracted from it so that texture 0 can be used!

        //WallX is the exact place on the wall that was hit
        double wallX; 
        if (side == 0) wallX = pos.y + perpWallDist * rayDir.y;
        else           wallX = pos.x + perpWallDist * rayDir.x;
        wallX -= std::floor(wallX);

        //x coordinate on the texture
        int texX = int(wallX * double(textures[texNum].width));
        if(side == 0 && rayDir.x > 0) texX = textures[texNum].width - texX - 1;
        if(side == 1 && rayDir.y < 0) texX = textures[texNum].width - texX - 1;

        // How much to increase the texture coordinate per screen pixel
        double texStep = 1.0 * textures[texNum].height / lineHeight;
        // Starting texture coordinate
        double texPos = (drawStart - mGraphics->SCREEN_HEIGHT / 2 + lineHeight / 2) * texStep;
        for(int y = drawStart; y<drawEnd; y++)
        {
            // Cast the texture coordinate to integer, and mask with the height of the texture - 1 in case of overflow
            int texY = (int)texPos & (textures[texNum].height - 1);
            texPos += texStep;

            color = textures[texNum].GetPixel(texX, texY);

            if(side == 1)
            {
                color.r = color.r / 2;
                color.g = color.g / 2;
                color.b = color.b / 2;
            }
            mGraphics->buffer[y][x] = color;
        }
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
    if(mInput->KeyPressed(SDL_SCANCODE_UP) && currentLevel < maxLevels)
    {
        currentLevel++;
        levelData = "res/maps/level" + std::to_string(currentLevel) + ".data";
        LoadMap(level, levelData);
        // pos.x += 1.0f;
        pos = level.playerPosition;
    }
    if(mInput->KeyPressed(SDL_SCANCODE_DOWN) && currentLevel > 1)
    {
        currentLevel--;
        levelData = "res/maps/level" + std::to_string(currentLevel) + ".data";
        LoadMap(level, levelData);
        pos = level.playerPosition;
    }
}