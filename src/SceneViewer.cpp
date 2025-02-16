#include "SceneViewer.h"

#include "SB/Scene.h"
#include "SB/DebugLog.h"

void SceneViewer::DrawNode(SB::SceneNode* node, bool isRoot)
{
	if (isRoot) {
		for (const auto& childNode : node->m_Children)
		{
			DrawNode(childNode, false);
		}
		return;
	}

	m_VisibleNodes++;

	bool isSelected = (selectedNode == node);

	// Always expand the root node
	ImGuiTreeNodeFlags flags = isSelected ? ImGuiTreeNodeFlags_Selected : 0;
	if (isRoot)
	{
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
	}

	if (ImGui::TreeNodeEx(node->GetName().c_str(), flags))
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
			ImGui::SetDragDropPayload("NODE_PAYLOAD", &node, sizeof(SB::SceneNode*));
			ImGui::Text("%s", node->GetName().c_str());
			ImGui::EndDragDropSource();
		}

		// Recursion >:(
		for (const auto& childNode : node->m_Children)
		{
			DrawNode(childNode, false);
		}

		ImGui::TreePop();
	}

	const ImGuiPayload* payload = ImGui::GetDragDropPayload();
	if (payload == nullptr)	return;

	SB::SceneNode* droppedNode = *(SB::SceneNode**)payload->Data;
	if (droppedNode == nullptr) return;
		
	// Check if dropped node is a descendant of current node
	// Hardestr thing to implement i swear, but is so needed
	if (IsDescendant(droppedNode, node)) return;
			
	if (ImGui::BeginDragDropTarget()) 
	{
		if (ImGui::IsMouseReleased(0))
		{
			ImGui::AcceptDragDropPayload("NODE_PAYLOAD");
			SB::Scene::GetInstance().MoveNodeToParent(droppedNode, node);
		}

		ImGui::EndDragDropTarget();
	}
}

bool SceneViewer::IsDescendant(SB::SceneNode* potentialAncestor, SB::SceneNode* node)
{
	while (node != nullptr) 
	{
		if (node == potentialAncestor) 
		{
			return true;
		}
		const SB::SceneNode* nodeParent = node->GetParent();
		node = const_cast<SB::SceneNode*>(nodeParent);
	}
	return false;
}

void SceneViewer::Draw()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400, 200), ImVec2(FLT_MAX, FLT_MAX));
	ImGui::Begin("Scene Hierarchy");

	m_VisibleNodes = 0;

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();

	bool hovered = ImGui::IsMouseHoveringRect(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y));

	static ImVec2 popupPrevPos = ImVec2(0.0, 0.0);
	SB::SceneNode* rootNode = SB::Scene::GetInstance().GetRootNode();

	if (hovered && ImGui::IsMouseDoubleClicked(0))
	{
		selectedNode = nullptr;
	}

	// Draw the root node, recursively draws everything
	DrawNode(rootNode, true);

	if (hovered)
	{
		if (ImGui::IsMouseClicked(1) && !contextMenuOpen)
		{
			contextMenuOpen = true;
			popupPrevPos = ImGui::GetMousePos();

			if (selectedNode != nullptr)
			{
				SB::DEBUG_LOG("Selected node: %s", selectedNode->GetName().c_str());
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
			SB::SceneNode* p_SelectedNode = (selectedNode == nullptr) ? rootNode : selectedNode;

			if (selectedNode != nullptr)
			{
				if (ImGui::MenuItem("Move to root"))
				{
					SB::Scene::GetInstance().MoveNodeToParent(p_SelectedNode, SB::Scene::GetInstance().GetRootNode());
				}
				ImGui::Separator();
			}

			if (ImGui::BeginMenu("Add mesh..."))
			{
				if (ImGui::MenuItem("Plane"))
				{
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

void SceneViewer::RenderPropertyWindow(const SB::SceneNode* node)
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400, 200), ImVec2(FLT_MAX, FLT_MAX));
	ImGui::Begin("Entity Properties");

	if (node == nullptr)
	{
		ImGui::Text("No node selected");
		ImGui::End();
		return;
	}

	ImGui::Text("%s", node->GetName().c_str());

	ImGui::End();
}
