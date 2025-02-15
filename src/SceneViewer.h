#pragma once
#include <imgui.h>
#include <SB/Scene.h>

class SceneViewer
{
public:
	void Draw();

private:
	void DrawNode(SB::SceneNode* node, bool isRoot = false);

    SB::SceneNode* selectedNode = nullptr;
	void RenderPropertyWindow(const SB::SceneNode* node);
	ImVec2 contextMenuPos = ImVec2(0, 0);
	bool contextMenuOpen = false;
};
