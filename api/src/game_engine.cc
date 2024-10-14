#include "game_engine.h"
#include <./imgui.h>
#include <./imgui_impl_sdl2.h>
#include <./imgui_impl_sdlrenderer2.h>

#include "circle.h"


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

    //Begin():
    Rectangle rectangle1(core::Vec2i(0, 0), 200, 80, SDL_Color{ 255, 255, 255, 255 });
    Rectangle rectangle2(core::Vec2i(210, 0), 300, 80, SDL_Color{ 255, 255, 255, 255 });
    Rectangle rectangle3(core::Vec2i(520, 0), 200, 80, SDL_Color{ 255, 255, 0, 255 });
    Rectangle rectangle4(core::Vec2i(0, 0), 50, 50, SDL_Color{ 255, 0, 255, 255 });
    Rectangle rectangle5(core::Vec2i(0, 300), 500, 80, SDL_Color{ 0, 0, 255, 255 });
    rectangles_.emplace_back(rectangle1);
    rectangles_.emplace_back(rectangle2);
    rectangles_.emplace_back(rectangle3);
    rectangles_.emplace_back(rectangle4);
    rectangles_.emplace_back(rectangle5);

    Circle test_circle(core::Vec2i(300, 200), 60, core::Vec2i(300,300), 50.0f, 20.0f, SDL_Color{ 0, 0, 255, 255 });
    Circle test_circle2(core::Vec2i(50, 50), 20, core::Vec2i(300,300), 10.0f, 5.0f, SDL_Color{ 100, 100, 25, 255 });
    circles_.emplace_back(test_circle);
    circles_.emplace_back(test_circle2);

    //Update:
    while (is_running_)
    {

        HandleEvents();

        // Start new ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Render ImGui window with a checkbox
        if (show_test_window_) {
            ImGui::ShowDemoWindow();
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

        // Here we draw stuff
        for (auto r : rectangles_)
        {
            r.Draw(renderer_->GetSDLRenderer());
        }
        for (auto c : circles_)
        {
            c.UpdateOrbit();
        }
        for (auto c : circles_)
        {
            c.Draw(renderer_->GetSDLRenderer());
        }

        // Render ImGui on top of SDL renderer
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        renderer_->Draw();
    }
}

