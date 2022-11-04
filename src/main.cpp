/*
* TODO:
* Flip maps "x" and "y", to be inline with the vector2,
* Positions are weird and bugged; Possibly related to above issue. Might be a problem with map loader
* Something weird going on with non-square textures
* Transparency doesn't work (?)
* Modify default texture loading 
* Aspect ratio deforms the display
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