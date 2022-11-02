#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <stb_image.h>

struct Texture
{
    int width, height, channels;

    unsigned char* data;
    SDL_Color GetPixel(unsigned int x, unsigned int y)
    {
        int pixel = x*channels + y*width*channels;
        SDL_Color color;
        color.r = (unsigned char)data[pixel];
        color.g = (unsigned char)data[pixel + 1];
        color.b = (unsigned char)data[pixel + 2];
        if(channels > 3)
            color.a = (unsigned char)data[pixel + 3];
        else
            color.a = 255;

        return color;
    }
};