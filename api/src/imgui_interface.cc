#include <imgui_interface.h>


void ImGuiInterface::Initialize(Display* display)
{
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
    if (show_imgui) {
        ImGui::ShowDemoWindow();
        //Create a window called "My First Tool", with a menu bar.
        ImGui::Begin("Dagobert", nullptr, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
                if (ImGui::MenuItem("Close", "Ctrl+W"))  { show_imgui = false; }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
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
