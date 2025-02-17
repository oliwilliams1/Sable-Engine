#pragma once
#include <imgui.h>
#include <SB/Scene.h>

class SceneViewer
{
public:
	void Draw();

private:
    SB::SceneNode* selectedNode = nullptr;

	void DrawNode(SB::SceneNode* node, bool isRoot = false);
	void DrawAlternatingRows() const;

	bool IsDescendant(SB::SceneNode* potentialAncestor, SB::SceneNode* node);

	int m_VerticalNodesAmnt = 0;
	ImVec2 m_NodeFirstPos = ImVec2(0.0f, 0.0f);
	float m_NodeVertSize = 20.0f;

	ImVec2 selectedNodePos = ImVec2(0.0f, 0.0f);
	void DrawSelectedNodeHighlight();

	bool addComponentModal = false;
	bool m_MontextMenuOpen = false;
	void RenderPropertyWindow(SB::SceneNode* node);
};
