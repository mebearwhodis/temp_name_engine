#include "game_engine.h"

GameEngine::GameEngine()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window_ = new Window("Game Engine", 800, 600);
    renderer_ = new GraphicsRenderer(window_->GetSDLWindow());
    is_running_ = true;
}

GameEngine::~GameEngine()
{
    delete window_;
    delete renderer_;
    SDL_Quit();
}

void GameEngine::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            is_running_ = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                is_running_ = false;
            }
        }
        else if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                window_->Resize(event.window.data1, event.window.data2);
            }
        }
    }
}

void GameEngine::Run()
{
    while (is_running_)
    {
        HandleEvents();
        renderer_->Clear();
        renderer_->DrawRectangle(100, 100, 200, 200);
        renderer_->DrawRectangle(0, 0, 100, 100);
        renderer_->Present();
    }
}

