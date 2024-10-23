#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H
#include <SDL_render.h>
#include <SDL_video.h>

class Display
{
private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;

    int window_width_ = 1200;
    int window_height_ = 800;

public:

    Display();
    ~Display();

    [[nodiscard]] SDL_Window* window() const { return window_; }
    [[nodiscard]] SDL_Renderer* renderer() const { return renderer_; }

    void Clear() const;
};

#endif //GAME_WINDOW_H
