#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include <SDL2/SDL.h>

class FPSCounter
{
public:
    FPSCounter()
    : lastTime(SDL_GetTicks()), frameCount(0), fps(0.0f)
    {
    }

    void Update()
    {
        frameCount++;
        Uint32 currentTime = SDL_GetTicks();
        delta_time_ = currentTime - lastTime;

        if (delta_time_ > 1000) {
            fps = frameCount / (delta_time_ / 1000.0f);
            frameCount = 0;
            lastTime = currentTime;
        }
    }
    void Reset()
    {
        lastTime = SDL_GetTicks();
        frameCount = 0;
        fps = 0.0f;
    }
    float GetFPS() const
    {
        return fps;
    }
    float GetDeltatime() {return delta_time_;}

private:
    Uint32 lastTime;
    Uint32 frameCount;
    float delta_time_;
    float fps;
};

#endif //FPS_COUNTER_H
