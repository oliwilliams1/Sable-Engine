#pragma once
#include <imgui.h>
#include <SB/Scene.h>

class SceneViewer
{
public:
	void Draw();

private:
	void DrawNode(const std::shared_ptr<SB::SceneNode>& node, std::shared_ptr<SB::SceneNode>& selectedNode, bool isRoot = false);

	ImVec2 contextMenuPos = ImVec2(0, 0);
	bool contextMenuOpen = false;
};