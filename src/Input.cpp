#include "Input.hpp"

Input* Input::sInstance = nullptr;

Input* Input::Instance()
{
    if (sInstance == nullptr)
        sInstance = new Input();

    return sInstance;
}

void Input::Release()
{
    delete sInstance;
    sInstance = nullptr;
}


Input::Input()
{
    mKeyboardState = SDL_GetKeyboardState(&mKeyLength);
    mPrevKeyboardState = new Uint8[mKeyLength];
    memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);
}

Input::~Input()
{
    delete[] mPrevKeyboardState;
    mPrevKeyboardState = nullptr;
}

void Input::UpdatePrevInput()
{
    memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);
}

bool Input::KeyDown(SDL_Scancode scanCode)
{
    return mKeyboardState[scanCode];
}

bool Input::KeyPressed(SDL_Scancode scanCode)
{
    return !mPrevKeyboardState[scanCode] && mKeyboardState[scanCode];
}

bool Input::KeyReleased(SDL_Scancode scanCode)
{
    return mPrevKeyboardState[scanCode] && !mKeyboardState[scanCode];
}