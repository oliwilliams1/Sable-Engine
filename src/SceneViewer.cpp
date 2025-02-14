#include "SceneViewer.h"
#include <imgui.h>

void SceneViewer::Draw()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400, 200), ImVec2(FLT_MAX, FLT_MAX));
	ImGui::Begin("Scene Heirachy");

	//const SB::SceneNode* rootNode = m_Scene->GetRoot();

	ImGui::End();
}
