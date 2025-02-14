#pragma once

#include "SB/Scene.h"

class SceneViewer
{
public:
	void Draw();

private:
	SB::Scene* m_Scene;
};