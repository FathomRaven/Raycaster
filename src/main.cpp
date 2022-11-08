/*
#################### RAYCASTER ####################
#         Written by FathomRaven on GitHub        #
#         Inspired heavily by lodev.com's         #
#            raycaster tutorial series            #
#         Art comes from OpenGameArt.com          #
#             Uses SDL2 for rendering             #
###################################################

Art sources:

metal_ceiling.png (4-light-grey-metal-plates) by Georges "TRaK" Grondin:
https://opengameart.org/content/4-light-grey-metal-plates
metal.png (rusty metal floor) by Sindwiller:
https://opengameart.org/content/rusty-metal-floor-texture
bone_wall.png & bone_wall_pillar.png by p0ss:
https://opengameart.org/content/skull-wall
bricks.png by alpha_rats:
https://opengameart.org/content/bricks-tiled-texture-64x64
stonewall.png by West:
https://opengameart.org/node/8041
*/

/*
TODO:
Flip maps "x" and "y", to be inline with the vector2,
Positions are weird and bugged; Possibly related to above issue. Might be a problem with map loader
Something weird going on with non-square textures
Add in transparency support
Modify default texture loading 
Raycaster is stretched due to changing with and height
Sprite Z position does not function correctly
  Can not easily play sprite on to floor
  Update: Made some progress, but very finicky and annoying
*/

#include <iostream>
#include <string>
#include <vector>

#include "Timer.hpp"
#include "Math.hpp"
#include "Input.hpp"
#include "Graphics.hpp"
#include "Raycaster.hpp"

int main(int argc, char ** argv){

    Graphics* mGraphics = Graphics::Instance();
    Input* mInput = Input::Instance();
    Timer* mTimer = Timer::Instance();

    Raycaster raycaster;

    SDL_Event event;
    bool isRunning = true;
    while (isRunning)
    {
        mTimer->Update();

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;
            
            default:
                break;
            }
        }

        if(mTimer->DeltaTime() >= (1.0f / 60))
        {
            mTimer->Reset();
            mGraphics->ClearBackBuffer();

            raycaster.Render();
            raycaster.Update();
            
            mGraphics->Render();
            mInput->UpdatePrevInput();
        }
    }
    return 0;
}