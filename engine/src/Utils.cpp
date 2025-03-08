#include "Utils.h"

std::filesystem::path GetRelPath(std::string otherPath)
{
	std::filesystem::path relPath;
	std::filesystem::path currentPath = std::filesystem::current_path();
	std::filesystem::path parentPath = currentPath.parent_path();
	std::filesystem::path other_path(otherPath);

	std::filesystem::path full_other_path = parentPath / other_path;

	if (std::filesystem::exists(other_path))
	{
		// If in a build dir
		return currentPath / other_path;
	}
	else
	{
		// Relative to executable
		return full_other_path;
	}
}
