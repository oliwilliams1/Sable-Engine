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

	if (selectedNode == node)
	{
		selectedNodePos = ImGui::GetCursorScreenPos();
	}

	m_VerticalNodesAmnt++;

	bool isSelected = (selectedNode == node);

	if (isRoot) return;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	bool expanded = ImGui::TreeNodeEx(node->GetName().c_str(), flags);

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

	if (expanded)
	{
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

	m_VerticalNodesAmnt = 0;

	m_NodeFirstPos = ImGui::GetCursorScreenPos();
	m_NodeVertSize = ImGui::GetTreeNodeToLabelSpacing() - 2.0f * ImGui::GetStyle().FramePadding.y;

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();

	bool hovered = ImGui::IsMouseHoveringRect(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y));

	static ImVec2 popupPrevPos = ImVec2(0.0, 0.0);
	SB::SceneNode* rootNode = SB::Scene::GetInstance().GetRootNode();

	if (hovered && ImGui::IsMouseDoubleClicked(0))
	{
		selectedNode = nullptr;
	}

	DrawNode(rootNode, true);

	if (hovered)
	{
		if (ImGui::IsMouseClicked(1) && !m_MontextMenuOpen)
		{
			m_MontextMenuOpen = true;
			popupPrevPos = ImGui::GetMousePos();

			if (selectedNode != nullptr)
			{
				SB::SABLE_LOG("Selected node: %s", selectedNode->GetName().c_str());
			}
		}
	}

	DrawAlternatingRows();

	DrawSelectedNodeHighlight();

	ImGui::End();

	if (m_MontextMenuOpen)
	{
		ImGui::OpenPopup("ContextMenu");

		if (ImGui::BeginPopup("ContextMenu"))
		{
			ImVec2 newPos = ImGui::GetWindowPos();

			if (popupPrevPos.x != newPos.x && popupPrevPos.y != newPos.y)
			{
				m_MontextMenuOpen = false;
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
				m_MontextMenuOpen = false;
				selectedNode = nullptr;
			}

			ImGui::EndPopup();
		}
	}

	RenderPropertyWindow(selectedNode);
}

void SceneViewer::DrawAlternatingRows() const
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 currentPos = m_NodeFirstPos;
	float verticalPadding = ImGui::GetStyle().ItemSpacing.y;

	float paddingX = ImGui::GetStyle().FramePadding.x;
	float windowSizeX = ImGui::GetWindowSize().x - 2.0f * paddingX;


	for (int i = 0; i < m_VerticalNodesAmnt; i++)
	{
		bool draw = (i % 2 != 0);

		if (draw)
		{
			drawList->AddRectFilled(currentPos, ImVec2(currentPos.x + windowSizeX, currentPos.y + m_NodeVertSize), ImColor(1.0f, 1.0f, 1.0f, 0.05f));
		}

		currentPos.y += m_NodeVertSize + verticalPadding / 2.0f;
	}
}

void SceneViewer::DrawSelectedNodeHighlight()
{
	if (selectedNode == nullptr) return;

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	selectedNodePos.x = m_NodeFirstPos.x;

	float paddingX = ImGui::GetStyle().FramePadding.x;
	float windowSizeX = ImGui::GetWindowSize().x - 2.0f * paddingX;

	drawList->AddRectFilled(selectedNodePos, ImVec2(selectedNodePos.x + windowSizeX, selectedNodePos.y + m_NodeVertSize), ImColor(1.0f, 1.0f, 1.0f, 0.2f));
}

void SceneViewer::RenderPropertyWindow(SB::SceneNode* node)
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2(FLT_MAX, FLT_MAX));
	ImGui::Begin("Entity Properties");

	if (node == nullptr)
	{
		ImGui::Text("No node selected");
		ImGui::End();
		return;
	}

	static char nameBuffer[256];
	strncpy(nameBuffer, node->GetName().c_str(), sizeof(nameBuffer) - 1);
	nameBuffer[sizeof(nameBuffer) - 1] = '\0';

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - 122 - ImGui::GetStyle().ItemSpacing.x * 4);
	if (ImGui::InputText("##NodeName", nameBuffer, sizeof(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		node->SetName(nameBuffer);
	}

	const char* componentOptions[] = { "Mesh", "Script", "Int Variable", "Shader", "Rigid Body" };
	static int selectedComponent = 0;

	ImGui::SameLine();
	ImGui::SetNextItemWidth(100);
	ImGui::Combo("##ComponentType", &selectedComponent, componentOptions, IM_ARRAYSIZE(componentOptions));

	ImGui::SameLine();
	if (ImGui::Button("+", ImVec2(22, 0)))
	{
		
	}

	ImGui::End();
}
