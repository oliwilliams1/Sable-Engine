#include "Utils.h"

std::filesystem::path GetRelPath(std::string otherPath)
{
	std::filesystem::path relPath;
	std::filesystem::path exec_path = std::filesystem::current_path().parent_path();
	std::filesystem::path other_path(otherPath);

	std::filesystem::path full_other_path = exec_path / other_path;

	if (std::filesystem::exists(full_other_path))
	{
		// If in a build dir
		return full_other_path;
	}
	else
	{
		// Relative to executable
		return other_path;
	}
}
