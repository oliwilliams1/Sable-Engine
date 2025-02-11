#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

class FileBrowser
{
public:
	FileBrowser();
	~FileBrowser();

	void Render();

private:

    enum SB_FILE_TYPE
    {
        SB_FOLDER = 0,
        SB_UNKNOWN = 1,
        SB_ASSET_OBJ = 2,
        SB_ASSET_MTL = 3,
        SB_ASSET_FBX = 4,
        SB_ASSET_IMAGE = 5,
        SB_ASSET_AUDIO = 6,
        SB_ASSET_TEXT = 7,
		SB_ASSET_FONT = 8
    };

    struct File
    {
        std::string name;
        std::filesystem::path path;
        SB_FILE_TYPE type;
    };

    SB_FILE_TYPE GetFileType(const std::filesystem::path& path);
    void RenderDirectory(const File& file);
    void UpdateCacheDirectoryFiles(const std::filesystem::path& path);
	std::filesystem::path m_CurrentPath;
	std::vector<File> m_Files;
};