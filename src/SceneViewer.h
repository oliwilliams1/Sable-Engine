#pragma once
#include <imgui.h>
#include <SB/Scene.h>

class SceneViewer
{
public:
	void Draw();

private:
	int m_VisibleNodes = 0;
    SB::SceneNode* selectedNode = nullptr;

	void DrawNode(SB::SceneNode* node, bool isRoot = false);

	bool IsDescendant(SB::SceneNode* potentialAncestor, SB::SceneNode* node);

	void RenderPropertyWindow(const SB::SceneNode* node);
	ImVec2 contextMenuPos = ImVec2(0, 0);
	bool contextMenuOpen = false;
};
