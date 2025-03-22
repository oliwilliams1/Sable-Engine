#pragma once
#include <filesystem>
#include <string>

// Gets path relative to executable or build dir (depends on dev mode or not)
std::filesystem::path GetRelPath(const std::string& otherPath);

std::string StripExtension(const std::string& str);