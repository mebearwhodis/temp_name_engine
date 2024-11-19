#include "game_engine.h"

#include <SDL_events.h>

#include <iostream>
#include <ostream>

#include "imgui_interface.h"
#include "random.h"


GameEngine::GameEngine()
{
    display_ = new Display();
    timer_ = new Timer();
    graphics_manager_ = new GraphicsManager();
    planet_system_ = new PlanetSystem();
    trigger_system_ = new TriggerSystem();
    collision_system_ = new CollisionSystem();
    friction_system_ = new FrictionSystem();
    is_running_ = true;
    imgui_interface_ = new ImGuiInterface();
    imgui_interface_->Initialize(display_, this);
}

GameEngine::~GameEngine()
{
    delete imgui_interface_;
    delete friction_system_;
    delete collision_system_;
    delete trigger_system_;
    delete planet_system_;
    delete graphics_manager_;
    delete display_;
}

void GameEngine::ChangeScene(const SystemScene new_sample)
{
    // Perform cleanup for the current scene
    switch (selected_scene_) {
    case SystemScene::PlanetSystemScene: // Planet System
        planet_system_->Clear(); // Hypothetical cleanup method
        break;
    case SystemScene::TriggerSystemScene: // Trigger System
        trigger_system_->Clear(); // Cleanup if needed
        break;
    case SystemScene::CollisionSystemScene: // Collision System
        collision_system_->Clear(); // Cleanup if needed
        break;
    case SystemScene::FrictionSystemScene: // Friction System
        friction_system_->Clear(); // Cleanup if needed
        break;
    default:
        break;
    }

    //Update to the new scene
    selected_scene_ = new_sample;

    // Initialize the new scene
    switch (selected_scene_) {
    case SystemScene::PlanetSystemScene: // Planet System
        planet_system_->Initialize();
        break;
    case SystemScene::TriggerSystemScene: // Trigger System
        trigger_system_->Initialize();
        break;
    case SystemScene::CollisionSystemScene: // Collision System
        collision_system_->Initialize();
        break;
    case SystemScene::FrictionSystemScene: // Friction System
        friction_system_->Initialize();
        break;
    default:
        break;
    }
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
        else if (event.type == SDL_MOUSEBUTTONDOWN && !ImGui::GetIO().WantCaptureMouse)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                if (selected_scene_ == SystemScene::PlanetSystemScene)
                {
                    // PLANET SYSTEM:
                    planet_system_->ToggleSpawner();
                }
                if (selected_scene_ == SystemScene::FrictionSystemScene)
                {
                    int mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    const auto mouse_pos = math::Vec2f(static_cast<float>(mouse_x), static_cast<float>(mouse_y));
                    friction_system_->SpawnShape(mouse_pos, math::ShapeType::kCircle);
                }
            }
            else if (event.button.button == SDL_BUTTON_RIGHT && !ImGui::GetIO().WantCaptureMouse)
            {
                if (selected_scene_ == SystemScene::FrictionSystemScene)
                {
                    int mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    const auto mouse_pos = math::Vec2f(static_cast<float>(mouse_x), static_cast<float>(mouse_y));
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
    ChangeScene(selected_scene_);
    //Begin():
    const float fixed_time_step = timer_->FixedDeltaTime();
    float accumulator = 0.0f;

    while (is_running_)
    {
        // Handle events
        HandleEvents();
        imgui_interface_->Update(is_running_);

        // Update the timer
        timer_->Tick();
        float delta_time = timer_->DeltaTime();
        accumulator += delta_time;


// Fixed Time Step Update
        while (accumulator >= fixed_time_step)
        {
            // Update all systems with the fixed time step
            if (selected_scene_ == SystemScene::PlanetSystemScene)
            {
                planet_system_->Update(fixed_time_step * imgui_interface_->speed_multiplier() * 1000.0f, imgui_interface_->planets_colour());
            }
            else if (selected_scene_ == SystemScene::TriggerSystemScene)
            {
                trigger_system_->Update(fixed_time_step * imgui_interface_->speed_multiplier());
            }
            else if (selected_scene_ == SystemScene::CollisionSystemScene)
            {
                collision_system_->Update(fixed_time_step * imgui_interface_->speed_multiplier());
            }
            else if (selected_scene_ == SystemScene::FrictionSystemScene)
            {
                friction_system_->Update(fixed_time_step * imgui_interface_->speed_multiplier() * 4.f);
            }

            accumulator -= fixed_time_step; // Decrease the accumulator
        }

        // Render
        display_->Clear();
        graphics_manager_->Clear();

        // Render all systems based on the current state
        if (selected_scene_ == SystemScene::PlanetSystemScene)
        {
            graphics_manager_->CreateCircle(planet_system_->star()->position(), 10.f, SDL_Color(255,255,255,150) , false);
            for (auto p : planet_system_->planets())
            {
                graphics_manager_->CreateCircle(p.position(), p.radius(), p.color(), false);
            }
        }
        else if (selected_scene_ == SystemScene::TriggerSystemScene)
        {
            for (auto g : trigger_system_->objects())
            {
                switch (g.collider().GetShapeType())
                {
                case math::ShapeType::kAABB:
                    graphics_manager_->CreateAABB(g.collider().GetBoundingBox().min_bound(),
                                                  g.collider().GetBoundingBox().max_bound(), g.color(), true);
                    break;
                case math::ShapeType::kCircle:
                    graphics_manager_->CreateCircle(g.position(), g.radius(), g.color(), false);
                    break;
                default:
                    break;
                }
            }
            if(imgui_interface_->show_quadtree()){trigger_system_->quadtree()->Draw(display_->renderer());}
        }
        else if (selected_scene_ == SystemScene::CollisionSystemScene)
        {
            for (auto g : collision_system_->objects())
            {
                switch (g.collider().GetShapeType())
                {
                case math::ShapeType::kAABB:
                    graphics_manager_->CreateAABB(g.collider().GetBoundingBox().min_bound(),
                                                  g.collider().GetBoundingBox().max_bound(), g.color(), true);
                    break;
                case math::ShapeType::kCircle:
                    graphics_manager_->CreateCircle(g.position(), g.radius(), g.color(), false);
                    break;
                default:
                    break;
                }
            }
            if(imgui_interface_->show_quadtree()){collision_system_->quadtree()->Draw(display_->renderer());}
        }
        else if (selected_scene_ == SystemScene::FrictionSystemScene)
        {
            for (auto g : friction_system_->objects())
            {
                switch (g.collider().GetShapeType())
                {
                case math::ShapeType::kAABB:
                    graphics_manager_->CreateAABB(g.collider().GetBoundingBox().min_bound(),
                                                  g.collider().GetBoundingBox().max_bound(), g.color(), true);
                    break;
                case math::ShapeType::kCircle:
                    graphics_manager_->CreateCircle(g.position(), g.radius(), g.color(), false);
                    break;
                default:
                    break;
                }
            }
            if(imgui_interface_->show_quadtree()){friction_system_->quadtree()->Draw(display_->renderer());}
        }

        // Render the graphics
        SDL_RenderGeometry(display_->renderer(),
                           nullptr,
                           graphics_manager_->vertices().data(),
                           static_cast<int>(graphics_manager_->vertices().size()),
                           graphics_manager_->indices().data(),
                           static_cast<int>(graphics_manager_->indices().size()));

        imgui_interface_->Render();
        SDL_RenderPresent(display_->renderer());
    }
    // End()
}
