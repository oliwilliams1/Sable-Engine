#pragma once
#include <imgui.h>

class SceneViewer
{
public:
	void Draw();

private:
	ImVec2 contextMenuPos = ImVec2(0, 0);
	bool contextMenuOpen = false;
};