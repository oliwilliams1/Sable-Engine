#pragma once
#include <SB/ProjectManager.h>

namespace EditorProjManager
{
	bool DisplayNewProjectWindow(bool* open, SB::SB_Project& project);
	void DisplayOpenProjectWindow(bool* open);
	void DisplaySaveProjectWindow(bool* open);

	bool LoadProject(const std::string& path, SB::SB_Project& project);
}