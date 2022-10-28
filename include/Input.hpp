#pragma once

#include <SDL2/SDL.h>
#include <string.h>

class Input
{    
public:
    static Input* Instance();
    static void Release();

    bool KeyDown(SDL_Scancode scanCode);
    bool KeyPressed(SDL_Scancode scanCode);
    bool KeyReleased(SDL_Scancode scanCode);

    void Update();
    void UpdatePrevInput();

private:
    static Input* sInstance;

    Uint8* mPrevKeyboardState;
    const Uint8* mKeyboardState;
    int mKeyLength;
    
    Input();
    ~Input();
};