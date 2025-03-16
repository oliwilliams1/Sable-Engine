#include "Utils.h"

std::filesystem::path GetRelPath(std::string otherPath)
{
	std::filesystem::path currentPath = std::filesystem::current_path();

	// crawl all subfolders until we find otherpath in subdirectories

	if (std::filesystem::exists(currentPath / otherPath))
	{
		return currentPath / otherPath;
	}

	std::vector<std::filesystem::path> subdirectories;

	for (const auto& entry : std::filesystem::directory_iterator(currentPath))
	{
		if (entry.is_directory())
		{
			subdirectories.push_back(entry.path());
		}
	}

	for (const auto& subdirectory : subdirectories)
	{
		if (std::filesystem::exists(subdirectory / otherPath))
		{
			return subdirectory / otherPath;
		}
	}
}