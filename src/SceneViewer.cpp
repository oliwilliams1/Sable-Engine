#include "SceneViewer.h"

#include "SB/Scene.h"
#include "SB/DebugLog.h"

void SceneViewer::DrawNode(const std::shared_ptr<SB::SceneNode>& node, std::shared_ptr<SB::SceneNode>& selectedNode, bool isRoot)
{
    // Cant be bothered backtracking, this will remove root node, i will probably revert later
    if (isRoot) {
        for (const auto& childNode : node->m_Children)
        {
            DrawNode(childNode, selectedNode, false);
        }
        return;
    }

    bool isSelected = (selectedNode == node);

    // Always expand the root node
    ImGuiTreeNodeFlags flags = isSelected ? ImGuiTreeNodeFlags_Selected : 0;
    if (isRoot)
    {
        flags |= ImGuiTreeNodeFlags_DefaultOpen;
    }

    if (ImGui::TreeNodeEx(node->m_Name.c_str(), flags))
    {
        // Selection
        if (ImGui::IsItemClicked(0))
        {
            selectedNode = node;
        }
        else if (ImGui::IsItemClicked(1))
        {
            selectedNode = node;
        }

        // Begin dragging this node
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload("NODE_PAYLOAD", &node, sizeof(node));
            ImGui::Text("%s", node->m_Name.c_str());
            ImGui::EndDragDropSource();
        }

        // Recursion :(
        for (const auto& childNode : node->m_Children)
        {
            DrawNode(childNode, selectedNode);
        }

        ImGui::TreePop();
    }

    // Check if this node is a drop target
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE_PAYLOAD"))
        {
            auto& droppedNode = *(std::shared_ptr<SB::SceneNode>*)(payload->Data);
            // Allow dropping onto root or other nodes, but not onto itself
            if (droppedNode != node || node == SB::Scene::GetInstance().GetRoot())
            {
                SB::Scene::GetInstance().MoveNodeToParent(droppedNode, node);
            }
        }
        ImGui::EndDragDropTarget();
    }
}

void SceneViewer::Draw()
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 200), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("Scene Hierarchy");

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 windowPos = ImGui::GetWindowPos();

    bool hovered = ImGui::IsMouseHoveringRect(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y));

    static ImVec2 popupPrevPos = ImVec2(0.0, 0.0);
    const std::shared_ptr<SB::SceneNode> rootNode = SB::Scene::GetInstance().GetRoot();
    static std::shared_ptr<SB::SceneNode> selectedNode = nullptr;

    if (hovered && ImGui::IsMouseDoubleClicked(0))
    {
        selectedNode = nullptr;
    }

    // Draw the root node (always expanded)
    DrawNode(rootNode, selectedNode, true);

    if (hovered)
    {
        if (ImGui::IsMouseClicked(1) && !contextMenuOpen)
        {
            contextMenuOpen = true;
            popupPrevPos = ImGui::GetMousePos();

            if (selectedNode != nullptr)
            {
                SB::DEBUG_LOG("Selected node: %s", selectedNode->m_Name.c_str());
            }
        }
    }

    ImGui::End();

    if (contextMenuOpen)
    {
        ImGui::OpenPopup("ContextMenu");

        if (ImGui::BeginPopup("ContextMenu"))
        {
            ImVec2 newPos = ImGui::GetWindowPos();

            if (popupPrevPos.x != newPos.x && popupPrevPos.y != newPos.y)
            {
                contextMenuOpen = false;
            }

            popupPrevPos = newPos;

            bool menuItemClicked = false;
            std::shared_ptr<SB::SceneNode> p_SelectedNode = (selectedNode == nullptr) ? rootNode : selectedNode;

            if (selectedNode != nullptr)
            {
                if (ImGui::MenuItem("Move to root"))
                {
                    SB::Scene::GetInstance().MoveNodeToParent(p_SelectedNode, SB::Scene::GetInstance().GetRoot());
                }
                ImGui::Separator();
            }

            if (ImGui::BeginMenu("Add mesh..."))
            {
                if (ImGui::MenuItem("Plane"))
                {
                    SB::DEBUG_LOG("Parent node %s", p_SelectedNode->m_Name.c_str());
                    SB::Scene::GetInstance().AddNode("Plane", p_SelectedNode);
                    menuItemClicked = true;
                }
                if (ImGui::MenuItem("Cube"))
                {
                    SB::Scene::GetInstance().AddNode("Cube", p_SelectedNode);
                    menuItemClicked = true;
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Add light..."))
            {
                if (ImGui::MenuItem("Point light")) {}
                if (ImGui::MenuItem("Sun light")) {}
                ImGui::EndMenu();
            }

            ImGui::MenuItem("Add camera");

            if (menuItemClicked)
            {
                contextMenuOpen = false;
                selectedNode = nullptr;
            }

            ImGui::EndPopup();
        }
    }

    RenderPropertyWindow(selectedNode);
}

void SceneViewer::RenderPropertyWindow(const std::shared_ptr<SB::SceneNode>& node)
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 200), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("Entity Properties");

    if (node == nullptr)
    {
        ImGui::Text("No node selected");
        ImGui::End();
        return;
    }

    ImGui::Text("%s", node->m_Name.c_str());

    ImGui::End();
}