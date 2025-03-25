#include <imgui.h>
#include "ProjectManager.h"
#include "SB/DebugLog.h"
#include "Utils.h"

using namespace SB;

bool EditorProjManager::DisplayNewProjectWindow(bool* open, SB::SB_Project& inputProject)
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
            SABLE_LOG("Project name cannot be empty");
            ImGui::End();
            return false;
        }
        SB_Project projectData;
        std::string projectNameString = projectName;
        projectData.name = projectNameString;
        projectData.path = projectNameString;

        SaveProject(projectNameString + ".sbproj", projectData);

        *open = false;

        inputProject = SB_Project(projectData);

        ImGui::End();
        return true;
    }

    ImGui::End();
    return false;
}

bool EditorProjManager::LoadProject(const std::string& path, SB::SB_Project& project)
{
    SB::LoadProject(path + ".sbproj", project);
    LoadMeshesEditor(project.name);
    return true;
}
