#include <fstream>
#include <yaml-cpp/yaml.h>
#include "ProjectManager.h"
#include "DebugLog.h"
#include "Utils.h"

void SB::SaveProject(const std::string& filename, const SB_Project& projectData)
{
	std::filesystem::path relPath = GetRelPath("projects");

	YAML::Node projectNode;

	projectNode["name"] = projectData.name;
	projectNode["path"] = projectData.path;

	std::filesystem::path filenamePath = relPath / std::filesystem::path(filename);

	std::ofstream file(filenamePath.string());
	if (file.is_open())
	{
		file << projectNode;
		file.close();

		Console::Log("Initialized project: %s", filename.c_str());
	}
	else
	{
		Console::Error("Unable to initialize project: %s", filename.c_str());
	}
}

void SB::LoadProject(const std::string& filename, SB_Project& projectData)
{
	std::filesystem::path relPath = GetRelPath("projects");

	std::filesystem::path filenamePath = relPath / std::filesystem::path(filename);

	YAML::Node projectNode = YAML::LoadFile(filenamePath.string());

	if (!projectNode["name"])
	{
		Console::Error("Project at: %s does not have a name", filename.c_str());
	}
	if (!projectNode["path"])
	{
		Console::Error("Project at: %s does not have a path", filename.c_str());
	}

	projectData.name = projectNode["name"].as<std::string>();
	projectData.path = projectNode["path"].as<std::string>();
}