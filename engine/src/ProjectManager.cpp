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

		std::filesystem::create_directory(relPath / projectData.name);

		std::filesystem::create_directory(relPath / projectData.name / "meshes");
		std::filesystem::create_directory(relPath / projectData.name / "textures");
		std::filesystem::create_directory(relPath / projectData.name / "materials");

		DEBUG_LOG("Initialized project: %s", filename.c_str());
	}
	else
	{
		DEBUG_ERROR("Unable to initialize project: %s", filename.c_str());
	}
}

void SB::LoadProject(const std::string& filename, SB_Project& projectData)
{
	std::filesystem::path relPath = GetRelPath("projects");

	std::filesystem::path filenamePath = relPath / std::filesystem::path(filename);

	if (!std::filesystem::exists(filenamePath))
	{
		DEBUG_WARN("Project at: %s does not exist", filename.c_str());
		return;
	}

	YAML::Node projectNode = YAML::LoadFile(filenamePath.string());

	if (!projectNode["name"])
	{
		DEBUG_WARN("Project at: %s does not have a name", filename.c_str());
	}
	if (!projectNode["path"])
	{
		DEBUG_WARN("Project at: %s does not have a path", filename.c_str());
	}

	projectData.name = projectNode["name"].as<std::string>();
	projectData.path = projectNode["path"].as<std::string>();
}