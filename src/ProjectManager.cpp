#include <imgui.h>
#include "ProjectManager.h"
#include "SB/DebugLog.h"

using namespace SB;

void EditorProjManager::DisplayNewProjectWindow(bool* open)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_Modal;

    ImVec2 fixedSize(400, 600);

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 viewportSize = viewport->Size;
    ImVec2 windowPos((viewportSize.x - fixedSize.x) * 0.5f, (viewportSize.y - fixedSize.y) * 0.5f);

    // Might have to change for linux who knows
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x + windowPos.x, viewport->Pos.y + windowPos.y));
    ImGui::SetNextWindowSize(fixedSize);

    ImGui::Begin("New Project", open, window_flags);

    static char projectName[128] = "";
    ImGui::Text("Enter Project Name");
    ImGui::SameLine();
    ImGui::InputText("##Project Name", projectName, IM_ARRAYSIZE(projectName));

    if (ImGui::Button("Create"))
    {
        if (projectName[0] == '\0')
		{
            Console::Log("Project name cannot be empty");
            ImGui::End();
			return;
		}
        SB_Project projectData;
        std::string projectNameString = projectName;
        projectData.name = projectNameString;
        projectData.path = projectNameString;

        SaveProject(projectNameString + ".sbproj", projectData);
        ImGui::End();
        *open = false;
        return;
    }

    ImGui::End();
}