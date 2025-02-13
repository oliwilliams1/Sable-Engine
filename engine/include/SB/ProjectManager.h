#pragma once
#include <string>

namespace SB
{
	struct SB_Project
	{
		std::string name;
		std::string path;
	};

	void SaveProject(const std::string& filename, const SB_Project& projectData);
	void LoadProject(const std::string& filename, SB_Project& projectData);
}