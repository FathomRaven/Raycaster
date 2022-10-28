#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Graphics
{    
public:

    static const int SCREEN_WIDTH = 1024;
    static const int SCREEN_HEIGHT = 768;

    const char* WINDOW_TITLE = "Tutorial";

    static Graphics* Instance();
    static void Release();
    static bool Initialized();

    void ClearBackBuffer();
    void Render();
    void DrawLine(SDL_Color color, int startX, int startY, int endX, int endY);

private:
    static Graphics* sInstance;
    static bool sInitialized;

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    SDL_Surface* mBackBuffer;

    Graphics();
    ~Graphics();

    bool Init();
};