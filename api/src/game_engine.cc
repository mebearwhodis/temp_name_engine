#include "game_engine.h"
#include <./imgui_interface.h>
#include <ctime>
#include <iostream>
#include <ostream>
#include <SDL_events.h>

#include "random.h"


GameEngine::GameEngine()
{
    display_ = new Display();
    timer_ = new Timer();
    graphics_manager_ = new GraphicsManager();
    planet_system_ = new PlanetSystem();
    trigger_system_ = new TriggerSystem();
    friction_system_ = new FrictionSystem();
    is_running_ = true;
    imgui_interface_ = new ImGuiInterface();
    imgui_interface_->Initialize(display_);

    fps_counter_ = new FPSCounter();
}

GameEngine::~GameEngine()
{

    delete imgui_interface_;
    delete friction_system_;
    delete trigger_system_;
    delete planet_system_;
    delete graphics_manager_;
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
                if(selected_system_ == 0)
                {
                    // PLANET SYSTEM:
                    int mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    const auto mouse_pos = math::Vec2f(mouse_x, mouse_y);
                    planet_system_->SpawnPlanets(mouse_pos);
                }
                if(selected_system_ == 3)
                {
                    int mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    const auto mouse_pos = math::Vec2f(mouse_x, mouse_y);
                    friction_system_->SpawnShape(mouse_pos, math::ShapeType::kCircle);
                }
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                if(selected_system_ == 3)
                {
                    int mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    const auto mouse_pos = math::Vec2f(mouse_x, mouse_y);
                    friction_system_->SpawnShape(mouse_pos, math::ShapeType::kAABB);
                }
            }
        }
        // else if (event.type == SDL_MOUSEBUTTONUP)
        // {
        //     if (event.button.button == SDL_BUTTON_LEFT)
        //     {
        //     }
        // }
    }
}

void GameEngine::Run()
{
    //Begin():

    while (is_running_)
    {
        //Update()
        timer_->Tick();
        HandleEvents();

        imgui_interface_->Update(is_running_);

        display_->Clear();
        graphics_manager_->Clear();

        //PLANET SYSTEM:
        if(selected_system_ == 0)
        {
            std::vector<math::Vec2f> points = {{1100, 700},{800, 600},{1000, 400}, {1050, 300},{1100,400}};

            graphics_manager_->CreateAABB(math::Vec2f(50, 50), math::Vec2f(250, 550), SDL_Color(0, 0, 255, 255), true);
            graphics_manager_->CreateAABB(math::Vec2f(100, 100), math::Vec2f(150, 150), SDL_Color(0, 255, 0, 255), true);
            graphics_manager_->CreateAABB(math::Vec2f(300, 600), math::Vec2f(400, 800), SDL_Color(0, 255, 0, 255), true);

            graphics_manager_->CreatePolygon(points, math::Vec2f(1040,500), SDL_Color(0, 255, 255, 255), true);

            planet_system_->UpdatePlanetsSIMD();
             for(auto p : planet_system_->planets())
             {
                 graphics_manager_->CreateCircle(p.position(), p.radius(), p.color(), false);
             }
        }

        //TRIGGER SYSTEM:
        if(selected_system_ == 1)
        {
            trigger_system_->Update();
            for(auto g : trigger_system_->objects())
            {
                switch (g.collider().GetShapeType())
                {
                case math::ShapeType::kAABB:
                    graphics_manager_->CreateAABB(g.collider().GetBoundingBox().min_bound(), g.collider().GetBoundingBox().max_bound(), g.color(), true);
                    break;
                case math::ShapeType::kCircle:
                    graphics_manager_->CreateCircle(g.position(), g.radius(), g.color(), false);
                    break;
                case math::ShapeType::kPolygon:
                case math::ShapeType::kNone:
                default:
                    break;
                }
            }
            //TODO add imgui toggle for quadtree display
            trigger_system_->quadtree()->Draw(display_->renderer());
        }
        //COLLISION SYSTEM:
        if(selected_system_ == 2){}

        //FRICTION SYSTEM:
        if(selected_system_ == 3)
        {
            friction_system_->Update();
            for(auto g : friction_system_->objects())
            {
                switch (g.collider().GetShapeType())
                {
                case math::ShapeType::kAABB:
                    graphics_manager_->CreateAABB(g.collider().GetBoundingBox().min_bound(), g.collider().GetBoundingBox().max_bound(), g.color(), true);
                    break;
                case math::ShapeType::kCircle:
                    graphics_manager_->CreateCircle(g.position(), g.radius(), g.color(), false);
                    break;
                case math::ShapeType::kPolygon:
                case math::ShapeType::kNone:
                default:
                    break;
                }
            }
        }


        //Render
        SDL_RenderGeometry(display_->renderer(),
            nullptr,
            graphics_manager_->vertices().data(),
            graphics_manager_->vertices().size(),
            graphics_manager_->indices().data(),
            graphics_manager_->indices().size());



        // Update FPS counter
        //fps_counter_->Update();
        //std::cout << "FPS: " << fps_counter_->GetFPS() << std::endl;

        imgui_interface_->Render();
        SDL_RenderPresent(display_->renderer());
    }
    // End()
}

