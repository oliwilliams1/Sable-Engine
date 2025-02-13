#pragma once
#include <GL/glew.h>

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
    void SetPath(const std::filesystem::path& path);
    void SetCurrentProjectPath(const std::filesystem::path& path);
    void UpdateCache();

private:

    enum SB_FILE_TYPE
    {
        SB_ASSET_UNKNOWN = -1,
        SB_ASSET_FOLDER = 0,
        SB_ASSET_OBJ = 1,
        SB_ASSET_MTL = 2,
        SB_ASSET_FBX = 3,
        SB_ASSET_GLTF = 4,
        SB_ASSET_IMAGE = 5,
        SB_ASSET_SCRIPT = 6,
        SB_ASSET_DOCUMENT = 7,
        SB_ASSET_AUDIO = 8,
        SB_ASSET_CONFIG = 9,
        SB_ASSET_SHADER = 10,
        SB_ASSET_COMP_FOLDER = 11
    };

    struct File
    {
        std::string name;
        std::filesystem::path path;
        SB_FILE_TYPE type;
        GLuint icon;
    };

    int fileWidth = 125, fileHeight = 150;

    GLuint GetFileTexture(SB_FILE_TYPE type) const;

    SB_FILE_TYPE GetFileType(const std::filesystem::path& path);
    void UpdateCacheDirectoryFiles(const std::filesystem::path& path);
	std::filesystem::path m_CurrentPath;
    std::filesystem::path m_CurrentProjectPath;
    std::filesystem::path m_ProjectRelativePath;
	std::vector<File> m_Files;

    GLuint m_FolderTexture, m_ImageFileTexture, m_UnknownFileTexture, m_FolderCompressedTexture,
        m_DocumentFileTexture, m_ConfigFileTexture, m_ScriptFileTexture, m_AudioFileTexture,
        m_FBX_FileTexture, m_GLTF_FileTexture, m_OBJ_FileTexture, m_MTL_FileTexture;
};