#include "Timer.hpp"

Timer* Timer::sInstance = nullptr;

Timer* Timer::Instance()
{
    if (sInstance == nullptr)
        sInstance = new Timer();

    return sInstance;
}

void Timer::Release()
{
    delete sInstance;
    sInstance = nullptr;
}

Timer::Timer()
{
    Reset();
    mElapsedTicks = 0;
    mDeltaTime = 0.0f;
    mTimeScale = 1.0f;
}

Timer::~Timer()
{

}

void Timer::Update()
{
    mElapsedTicks = SDL_GetTicks() - mStartTicks;
    mDeltaTime = mElapsedTicks * 0.001;
}

void Timer::Reset()
{
    mStartTicks = SDL_GetTicks();
}

float Timer::DeltaTime()
{
    return mDeltaTime;
}

void Timer::TimeScale(float t)
{
    mTimeScale = t;
}

float Timer::TimeScale()
{
    return mTimeScale;
}