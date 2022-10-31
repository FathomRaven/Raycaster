#include "Graphics.hpp"

Graphics* Graphics::sInstance = nullptr;
bool Graphics::sInitialized = false;

Graphics* Graphics::Instance()
{
    if (sInstance == nullptr)
        sInstance = new Graphics();

    return sInstance;
}

void Graphics::Release()
{
    delete sInstance;
    sInstance = nullptr;

    sInitialized = false;
}

bool Graphics::Initialized()
{
    return sInitialized;
}

bool Graphics::Init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Init Error: %s\n", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(mWindow == nullptr)
    {
        printf("Window Creation Error: %s\n", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if(mRenderer == nullptr)
    {
        printf("Renderer Creation Error: %s\n", SDL_GetError());
        return false;
    }

    ClearBackBuffer();

    mBackBuffer = SDL_GetWindowSurface(mWindow);

    return true;
}

Graphics::Graphics()
{
    sInitialized = Init();
}

Graphics::~Graphics()
{
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;

    SDL_DestroyRenderer(mRenderer);
    mRenderer = nullptr;

    SDL_Quit();
}

void Graphics::ClearBackBuffer()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);
    for (unsigned int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (unsigned int x = 0; x < SCREEN_WIDTH; x++)
        {
            buffer[y][x] = {0, 0, 0, 0};
        }
    }
}

void Graphics::Render()
{
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(buffer, SCREEN_WIDTH, SCREEN_HEIGHT, 32, SCREEN_WIDTH*4, 0x000000ff,0x0000ff00,0x00ff0000,0xff000000);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);

    SDL_RenderCopy(mRenderer, texture, NULL, NULL);
    SDL_RenderPresent(mRenderer);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Graphics::DrawLine(SDL_Color color, int startX, int startY, int endX, int endY)
{
    SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(mRenderer, startX, startY, endX, endY);
}

void Graphics::DrawRectangle(SDL_Color color, SDL_Rect& rect)
{
    SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(mRenderer, &rect);
}