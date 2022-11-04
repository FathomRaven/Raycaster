#pragma once

#include <math.h>
#include <SDL2/SDL.h>

#define PI 3.14159265

struct Vector2
{
    float x, y;
    Vector2(float _x = 0.0f, float _y = 0.0f)
    :x(_x), y(_y) {}

    float MagnitudeSqr()
    {
        return x*x + y*y;
    }

    float Magnitude()
    {
        return (float)sqrt(x*x + y*y);
    }

    Vector2 Normalized()
    {
        float mag = Magnitude();

        return Vector2(x/mag, y/mag);
    }

    Vector2& operator +=(const Vector2& rhs)
    {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    Vector2& operator -=(const Vector2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }
};

inline Vector2 operator +(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline Vector2 operator -(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

inline Vector2 operator *(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
}

struct Vector2i
{
    int x, y;
    Vector2i(int _x = 0.0f, int _y = 0.0f)
    :x(_x), y(_y) {}
};