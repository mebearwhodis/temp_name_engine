#include "game_engine.h"
#include <./imgui_interface.h>
#include <ctime>

#include "circle.h"


GameEngine::GameEngine()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window_ = new Window("Game Engine", 1200, 800);
    renderer_ = new GraphicsRenderer(window_->GetSDLWindow());
    is_running_ = true;
    show_imgui_window_ = true;
    imgui_interface_ = new ImGuiInterface();
    imgui_interface_->Initialize(window_, renderer_);

    std::srand(std::time(nullptr)); // use current time as seed for random generator
}

GameEngine::~GameEngine()
{

    delete imgui_interface_;
    delete window_;
    delete renderer_;
    SDL_Quit();
}

void GameEngine::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // Pass events to ImGui for input handling
        imgui_interface_->PassEvents(event);

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
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int random_value = std::rand();
                Circle* test_circle = new Circle(math::Vec2i(0, 0), random_value%20, math::Vec2i(600,400), random_value%1000, 0.006f, SDL_Color{ 255, 13, 132, 255 });
                circles_.emplace_back(test_circle);
            }
        }
    }
}

void GameEngine::Run()
{
    //Begin():
    // Rectangle rectangle1(math::Vec2i(0, 0), 200, 80, SDL_Color{ 255, 255, 255, 255 });
    // rectangles_.emplace_back(rectangle1);
    //
    // Circle* test_circle = new Circle(math::Vec2i(300, 200), 60, math::Vec2i(800,600), 500.0f, 0.002f, SDL_Color{ 0, 0, 255, 255 });
    // circles_.emplace_back(test_circle);

    //Update():
    while (is_running_)
    {

        HandleEvents();

        imgui_interface_->Update(show_imgui_window_);

        renderer_->Clear();

        // Here we draw stuff
        for (auto r : rectangles_)
        {
            r.Draw(renderer_->GetSDLRenderer());
        }
        for (auto c : circles_)
        {
            c->UpdateOrbit();
        }
        for (auto c : circles_)
        {
            c->Draw(renderer_->GetSDLRenderer());
        }

        imgui_interface_->Render();

        renderer_->Draw();
    }
    // End()
    for (auto c : circles_)
    {
        delete c;
    }
}

