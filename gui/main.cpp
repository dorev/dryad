#include "imguiimplementation_sdl2opengl3.h"
#include "log.h"

int main(int argc, char* argv[])
{
    ImGuiImplementation<SDL2OpenGL3> imgui;
    ImGuiImplementationConfig config;
    config.name = "DryadGUI";
    bool shuttingDown = false;
    config.shuttingDown = &shuttingDown;
    int result = imgui.Init(config);

    while (!shuttingDown)
    {
        imgui.HandleEvents();
        imgui.NewFrame();
        static ImGuiIO& io = ImGui::GetIO();

        // Main window ============================================================================
        // ========================================================================================
        float windowWidth = io.DisplaySize.x;
        float windowHeight = io.DisplaySize.y;
        float topSection = windowHeight - bottomSection - ImGui::GetStyle().WindowPadding.y * 2;

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
        ImGuiWindowFlags mainWindowFlags = 0
            | ImGuiWindowFlags_NoDecoration
            | ImGuiWindowFlags_NoMove;

        ImGui::Begin("Dryad GUI", nullptr, mainWindowFlags);


        // Logs
        // ----------------------------------------------------------------------------------------
        ImGui::BeginChild("Logs", ImVec2(0, 0), ImGuiChildFlags_Border, ImGuiWindowFlags_None);
        ImGui::SeparatorText("Logs");
        const std::vector<std::unique_ptr<char[]>>& logs = Logger::GetLogLines();
        for (auto itr = logs.crbegin(); itr != logs.crend(); itr++)
            ImGui::Text("%s", itr->get());

        ImGui::EndChild();

        ImGui::End();
        // ========================================================================================
        // End of main window =====================================================================

        imgui.Render();
    }

    imgui.Shutdown();

    return 0;
}
