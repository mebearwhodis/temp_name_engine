#include "game_engine.h"
#include <./imgui_interface.h>
#include <ctime>
#include <iostream>
#include <ostream>
#include <SDL_events.h>



GameEngine::GameEngine()
{
    display_ = new Display();
    shape_manager_ = new GraphicalShape();
    planet_system_ = new PlanetSystem();
    trigger_system_ = new TriggerSystem();
    is_running_ = true;
    imgui_interface_ = new ImGuiInterface();
    imgui_interface_->Initialize(display_);
}

GameEngine::~GameEngine()
{

    delete imgui_interface_;
    delete trigger_system_;
    delete planet_system_;
    delete shape_manager_;
    delete display_;
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
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                // PLANET SYSTEM:
                // int mouse_x, mouse_y;
                // SDL_GetMouseState(&mouse_x, &mouse_y);
                // auto mouse_pos = math::Vec2f(mouse_x, mouse_y);
                // planet_system_->CreatePlanet(mouse_pos, 20);
            }
        }
    }
}

void GameEngine::Run()
{
    //Begin():



    while (is_running_)
    {
        //Update()

        HandleEvents();

        imgui_interface_->Update(is_running_);

        display_->Clear();
        shape_manager_->Clear();

        //PLANET SYSTEM:
        //planet_system_->UpdatePlanetsSIMD();
        // for(auto p : planet_system_->planets())
        // {
        //     shape_manager_->CreateCircle(p.position(), 50, SDL_Color{ 255, 13, 132, 255 });
        // }

        //TRIGGER SYSTEM:
        trigger_system_->UpdateShapes();
        for(auto g : trigger_system_->objects())
        {
            shape_manager_->CreateCircle(g.position(), g.radius(), g.color());
        }

        //Render
        SDL_RenderGeometry(display_->renderer(),
            nullptr,
            shape_manager_->vertices().data(),
            shape_manager_->vertices().size(),
            shape_manager_->indices().data(),
            shape_manager_->indices().size());


        imgui_interface_->Render();
        SDL_RenderPresent(display_->renderer());
    }
    // End()
}

