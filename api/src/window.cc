#include "window.h"

Window::Window(const std::string& title, int width, int height)
{
    window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    this->width_ = width;
    this->height_ = height;
}

Window::~Window()
{
    SDL_DestroyWindow(window_);
}

SDL_Window* Window::GetSDLWindow() const
{
    return window_;
}

void Window::Resize(int newWidth, int newHeight)
{
    this->width_ = newWidth;
    this->height_ = newHeight;
    SDL_SetWindowSize(window_, width_, height_);
}