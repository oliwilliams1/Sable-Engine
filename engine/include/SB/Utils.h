#pragma once
#include <filesystem>
#include <string>

// Gets path relative to executable or build dir (depends on dev mode or not)
std::filesystem::path GetRelPath(std::string otherPath);