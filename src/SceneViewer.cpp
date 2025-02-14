#include "SceneViewer.h"

#include "SB/Scene.h"
#include "SB/DebugLog.h"

void SceneViewer::Draw() 
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 200), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("Scene Hierarchy");

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 windowPos = ImGui::GetWindowPos();

    bool hovered = ImGui::IsMouseHoveringRect(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y));

    bool queueContextMenuClose = false;

    if (hovered) 
    {
        if (ImGui::IsMouseClicked(1) && !contextMenuOpen)
        {
            contextMenuPos = ImGui::GetMousePos();
			contextMenuOpen = true;
        }
    }

    const std::shared_ptr<SB::SceneNode> rootNode = SB::Scene::GetInstance().GetRoot();

    for (const auto& node : rootNode->m_Children)
    {
        ImGui::Selectable(node->m_Name.c_str());
    }

    ImGui::End();

    if (contextMenuOpen) 
    {
        ImGui::OpenPopup("ContextMenu");

        if (ImGui::BeginPopup("ContextMenu")) 
        {
            if (ImGui::BeginMenu("Add mesh...")) 
            {
                if (ImGui::MenuItem("Plane")) 
                {
                    SB::Scene::GetInstance().AddNode("Plane", rootNode);
                    contextMenuOpen = false;
                }
                if (ImGui::MenuItem("Cube")) 
                {
                    SB::Scene::GetInstance().AddNode("Cube", rootNode);
                    contextMenuOpen = false;
                }
                if (ImGui::MenuItem("Cone")) {}
                if (ImGui::MenuItem("Cylinder")) {}
                if (ImGui::MenuItem("Icosphere")) {}
                if (ImGui::MenuItem("Torus")) {}

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Add light...")) 
            {
                ImGui::MenuItem("Point light");
                ImGui::MenuItem("Sun light");
                ImGui::EndMenu();
            }

            ImGui::MenuItem("Add camera");

            ImGui::EndPopup();
        }
    }
}