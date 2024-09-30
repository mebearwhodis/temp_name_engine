#ifndef KUMAENGINE_API_WINDOW_H_
#define KUMAENGINE_API_WINDOW_H_

#include <string>
#include <SDL2/SDL_video.h>

class Window
{
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    SDL_Window* GetSDLWindow() const;
    void Resize(int width, int height);

private:
    SDL_Window* window_;
    int width_, height_;
};

#endif // KUMAENGINE_API_WINDOW_H_
