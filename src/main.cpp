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

    // Window app(SCREEN_WIDTH, SCREEN_HEIGHT);
    
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