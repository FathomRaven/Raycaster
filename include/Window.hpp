#include <SDL2/SDL.h>

class Window
{
public:
    Window(const int WIDTH, const int HEIGHT);
    ~Window();

    SDL_Window* window;
    SDL_Renderer* renderer;

    void Clear();

};