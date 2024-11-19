#include "imgui_interface.h"

#include <imgui_impl_sdlrenderer2.h>

#include "game_engine.h"

void ImGuiInterface::Initialize(Display* display, GameEngine* engine)
{
    game_engine_ = engine;

    //Setup ImGUI context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    //Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForSDLRenderer(display->window(), display->renderer());
    ImGui_ImplSDLRenderer2_Init(display->renderer());
}

void ImGuiInterface::Update(bool& show_imgui)
{
    //Start new ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //Render ImGui window with a checkbox
    if (show_imgui)
    {
        // Set the position and size of the ImGui window
        ImVec2 window_pos = ImVec2(0, 0);   // Set the position (x, y)
        ImVec2 window_size = ImVec2(300, 400); // Set the size (width, height)
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Once); // Position it once
        ImGui::SetNextWindowSize(window_size, ImGuiCond_Once); // Size it once

        // Create a window
        ImGui::Begin("Kuma Engine", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
        //TODO display quadtree or not + change other options maybe + reset scene button

        // Display FPS at the top
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

        ImGui::Separator();

        // Scene Selector Combo Box
        const char* scenes[] = {"Planet System", "Trigger System", "Collision System", "Friction System"};
        static int current_scene = 0; // Index of the current scene

        if (ImGui::BeginCombo("Scene Selector", scenes[current_scene]))
        {
            for (int n = 0; n < IM_ARRAYSIZE(scenes); n++)
            {
                const bool is_selected = (current_scene == n);
                if (ImGui::Selectable(scenes[n], is_selected))
                {
                    current_scene = n;
                    if(game_engine_)
                    {
                        game_engine_->ChangeScene(static_cast<SystemScene>(current_scene));
                    }
                }
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();

        // Scene-specific options
        switch (current_scene)
        {
        case 0: // Planet System
            {

                ImGui::PushTextWrapPos();
                ImGui::Text("Scene Description");
                ImGui::PopTextWrapPos();

                ImGui::Separator();

                static bool option_a = true;
                ImGui::Checkbox("Enable Option A", &option_a);


                ImGui::SliderFloat("Speed Multiplier", &speed_multiplier_, 1.0f, 10.0f);
                break;
            }
        case 1: // Trigger System
            {
                ImGui::PushTextWrapPos();
                ImGui::Text("Scene Description");
                ImGui::PopTextWrapPos();

                ImGui::Separator();

                ImGui::Checkbox("Show Quadtree", &show_quadtree_);

                ImGui::SliderFloat("Speed Multiplier", &speed_multiplier_, 0.0f, 10.0f);
                static int shape_count = 10;
                ImGui::SliderInt("Number of Shapes", &shape_count, 1, 100);
                break;
            }
        case 2: // Collision System
            {
                ImGui::PushTextWrapPos();
                ImGui::Text("Scene Description");
                ImGui::PopTextWrapPos();

                ImGui::Separator();

                ImGui::Checkbox("Show Quadtree", &show_quadtree_);

                ImGui::SliderFloat("Speed Multiplier", &speed_multiplier_, 0.0f, 10.0f);
                break;
            }
        case 3: // Friction System
            {
                ImGui::PushTextWrapPos();
                ImGui::Text("Scene Description");
                ImGui::PopTextWrapPos();

                ImGui::Separator();

                ImGui::Checkbox("Show Quadtree", &show_quadtree_);

                ImGui::SliderFloat("Speed Multiplier", &speed_multiplier_, 0.0f, 10.0f);
                break;
            }
        default:
            {
                ImGui::Text("No options available for this scene.");
                break;
            }
        }

        // End the window
        ImGui::End();
    }
}

void ImGuiInterface::Render()
{
    //Render ImGui on top of SDL renderer
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiInterface::PassEvents(SDL_Event& event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);
}

ImGuiInterface::~ImGuiInterface()
{
    //Cleanup ImGui
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
