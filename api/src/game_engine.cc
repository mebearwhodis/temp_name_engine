#include "game_engine.h"
#include <./imgui.h>
#include <./imgui_impl_sdl2.h>
#include <./imgui_impl_sdlrenderer2.h>


GameEngine::GameEngine()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window_ = new Window("Game Engine", 800, 600);
    renderer_ = new GraphicsRenderer(window_->GetSDLWindow());
    is_running_ = true;
    show_test_window_ = true;

    //Setup ImGUI context
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO();
      (void)io;

    //Setup Platform/Renderer bindings
     ImGui_ImplSDL2_InitForSDLRenderer(window_->GetSDLWindow(), renderer_->GetSDLRenderer());
    ImGui_ImplSDLRenderer2_Init(renderer_->GetSDLRenderer());
}

GameEngine::~GameEngine()
{
    // Cleanup ImGui
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

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
        ImGui_ImplSDL2_ProcessEvent(&event);

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
    static float my_color[4] = {0.4f, 0.7f, 0.0f, 1.0f};
    //Begin()
    while (is_running_)
    {

        HandleEvents();

        // Start new ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Render ImGui window with a checkbox
        if (show_test_window_) {
            // ImGui::ShowDemoWindow();
            // Create a window called "My First Tool", with a menu bar.
            ImGui::Begin("My First Tool", &show_test_window_, ImGuiWindowFlags_MenuBar);
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                    if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
                    if (ImGui::MenuItem("Close", "Ctrl+W"))  { show_test_window_ = false; }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            // Edit a color stored as 4 floats
            ImGui::ColorEdit4("Color",my_color);

            // Generate samples and plot them
            float samples[100];
            for (int n = 0; n < 100; n++)
                samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
            ImGui::PlotLines("Samples", samples, 100);

            // Display contents in a scrolling region
            ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
            ImGui::BeginChild("Scrolling");
            for (int n = 0; n < 50; n++)
                ImGui::Text("%04d: Some text", n);
            ImGui::EndChild();
            ImGui::End();
            ImGui::End();
        }

        ImGui::Render();

        renderer_->Clear();
        renderer_->DrawRectangle(0, 0, 200, 80, SDL_Color{ 255, 255, 255, 255 });
        renderer_->DrawRectangle(210, 0, 300, 80, SDL_Color{ 255, 255, 255, 255 });
        renderer_->DrawRectangle(520, 0, 200, 80, SDL_Color{ 255, 255, 0, 255 });
        renderer_->DrawRectangle(0, 0, 50, 50, SDL_Color{ 255, 0, 255, 255 });
        renderer_->DrawRectangle(0, 300, 500, 80, SDL_Color{ 0, 0, 255, 255 });

        renderer_->DrawCircle(400, 300, 60, SDL_Color{ 0, 255, 255, 255 });

        // Render ImGui on top of SDL renderer
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        renderer_->Draw();
    }
}

