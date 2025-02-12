#include <iostream>
#include <imgui.h>
#include "FileBrowser.h"
#include "SableEngine.h"

FileBrowser::FileBrowser()
{
	std::filesystem::path exec_path = std::filesystem::current_path().parent_path();
	std::filesystem::path other_path("resources");

	std::filesystem::path full_other_path = exec_path / other_path;

	if (std::filesystem::exists(full_other_path))
	{
		// If in a build dir
		m_CurrentPath = full_other_path;
	}
	else
	{
		// Relative to executable
		m_CurrentPath = other_path;
	}

	// Load all textures
	SBEngine& engine = SBEngine::GetInstance();
	engine.LoadTextureFromFile("icons/sb-icon-folder.png", m_FolderTexture);
	engine.LoadTextureFromFile("icons/sb-icon-image.png", m_ImageFileTexture);
	engine.LoadTextureFromFile("icons/sb-icon-unknown.png", m_UnknownFileTexture);
	engine.LoadTextureFromFile("icons/sb-icon-folder-compressed.png", m_FolderCompressedTexture);
	engine.LoadTextureFromFile("icons/sb-icon-document.png", m_DocumentFileTexture);
	engine.LoadTextureFromFile("icons/sb-icon-config.png", m_ConfigFileTexture);
	engine.LoadTextureFromFile("icons/sb-icon-code.png", m_ScriptFileTexture);
	engine.LoadTextureFromFile("icons/sb-icon-audio.png", m_AudioFileTexture);
	engine.LoadTextureFromFile("icons/sb-icon-fbx.png", m_FBX_FileTexture);
	engine.LoadTextureFromFile("icons/sb-icon-gltf.png", m_GLTF_FileTexture);
	engine.LoadTextureFromFile("icons/sb-icon-obj.png", m_OBJ_FileTexture);
	engine.LoadTextureFromFile("icons/sb-icon-mtl.png", m_MTL_FileTexture);
	
	UpdateCacheDirectoryFiles(m_CurrentPath);
}

GLuint FileBrowser::GetFileTexture(SB_FILE_TYPE type) const
{
	switch (type)
	{
	case SB_ASSET_FOLDER:
		return m_FolderTexture;

	case SB_ASSET_IMAGE:
		return m_ImageFileTexture;

	case SB_ASSET_OBJ:
		return m_OBJ_FileTexture;

	case SB_ASSET_MTL:
		return m_MTL_FileTexture;

	case SB_ASSET_FBX:
		return m_FBX_FileTexture;

	case SB_ASSET_GLTF:
		return m_GLTF_FileTexture;

	case SB_ASSET_SCRIPT:
		return m_ScriptFileTexture;

	case SB_ASSET_DOCUMENT:
		return m_DocumentFileTexture;

	case SB_ASSET_AUDIO:
		return m_AudioFileTexture;

	case SB_ASSET_CONFIG:
		return m_ConfigFileTexture;

	case SB_ASSET_COMP_FOLDER:
		return m_FolderCompressedTexture;

	default:
		return m_UnknownFileTexture;
	}
}

FileBrowser::SB_FILE_TYPE FileBrowser::GetFileType(const std::filesystem::path& path)
{
	if (std::filesystem::is_directory(path)) 
	{
		return SB_ASSET_FOLDER;
	}

	std::string extension = path.extension().string();

	if (extension == ".obj") 
	{
		return SB_ASSET_OBJ;
	}
	else if (extension == ".mtl") 
	{
		return SB_ASSET_MTL;
	}
	else if (extension == ".fbx") 
	{
		return SB_ASSET_FBX;
	}
	else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".bmp" || extension == "webp") 
	{
		return SB_ASSET_IMAGE;
	}
	else if (extension == ".wav" || extension == ".mp3" || extension == ".ogg" || extension == ".flac") 
	{
		return SB_ASSET_AUDIO;
	}
	else if (extension == ".txt")
	{
		return SB_ASSET_DOCUMENT;
	}
	else if (extension == ".vert" || extension == ".frag" || extension == ".comp" || extension == ".geom" || extension == ".tese" || extension == ".tesc" || extension == ".glsl")
	{
		return SB_ASSET_SHADER;
	}
	else if (extension == ".json" || extension == ".yaml" || extension == ".conf" || extension == ".ini")
	{
		return SB_ASSET_CONFIG;
	}

	return SB_ASSET_UNKNOWN;
}

void FileBrowser::UpdateCacheDirectoryFiles(const std::filesystem::path& path)
{
	std::vector<File> p_Files;

	if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			File file;
			file.name = entry.path().filename().string();
			file.path = entry.path();
			file.type = GetFileType(entry.path());
			file.icon = GetFileTexture(file.type);

			p_Files.emplace_back(file);
		}

		m_CurrentPath = path;
	}
	else 
	{
		return;
	}

	std::vector<File> p_Directories;
	for (const File& file : p_Files)
	{
		if (file.type == SB_ASSET_FOLDER)
		{
			p_Directories.emplace_back(file);
		}
	}

	// Remove directories from p_Files
	p_Files.erase(std::remove_if(p_Files.begin(), p_Files.end(), [](const File& file) {
		return file.type == SB_ASSET_FOLDER;
		}), p_Files.end());

	// Sort directories by first char in lowercase
	std::sort(p_Directories.begin(), p_Directories.end(), [](const File& a, const File& b) {
		return std::tolower(a.name.c_str()[0]) < std::tolower(b.name.c_str()[0]);
		});

	// Sort files by name in lowercase
	std::sort(p_Files.begin(), p_Files.end(), [](const File& a, const File& b) {
		return std::tolower(a.name.c_str()[0]) < std::tolower(b.name.c_str()[0]);
		});

	// Update files vector
	m_Files.clear();
	m_Files.push_back({ "..", m_CurrentPath.parent_path(), SB_ASSET_FOLDER, GetFileTexture(SB_ASSET_FOLDER)}); // Add parent dir button at start of vector
	m_Files.insert(m_Files.end(), p_Directories.begin(), p_Directories.end());
	m_Files.insert(m_Files.end(), p_Files.begin(), p_Files.end());
}

void FileBrowser::Render()
{
	ImGui::Text("%s", m_CurrentPath.string().c_str());

	ImVec2 window_size = ImGui::GetWindowSize();
	int width = (int)window_size.x;

	int padding = ImGui::GetStyle().ItemSpacing.x;
	int amntWide = (width - padding) / (fileWidth + padding);

	if (amntWide == 0)
	{
		amntWide = 1;
	}

	// Draw list to add some of my own pizazz (hover effect on a group)
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	float pady = ImGui::GetStyle().ItemSpacing.y;

	for (int i = 0; i < m_Files.size(); i++)
	{
		const File& file = m_Files[i];

		ImGui::BeginGroup();

		ImGui::Image((ImTextureID)(intptr_t)file.icon, ImVec2(fileWidth, fileWidth));

		ImVec2 pos = ImGui::GetItemRectMin();
		ImVec2 sizeImage = ImGui::GetItemRectSize();

		// Check if the image is hovered, and set the draw to true
		bool drawHover = false;
		if (ImGui::IsItemHovered())
		{
			drawHover = true;
		}

		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + fileWidth);

		// Render the text
		ImGui::TextWrapped(file.name.c_str());

		// Restore the previous wrap position
		ImGui::PopTextWrapPos();

		// Only need for height of text
		ImVec2 sizeText = ImGui::GetItemRectSize();

		// Size of rect = width of group (image & text), height of group (image + text + paddingy)
		ImVec2 size = ImVec2(pos.x + sizeImage.x, pos.y + sizeImage.y + sizeText.y + pady);

		// Draw hover after image and text
		if (drawHover)
		{
			// Outline + light fill (alpha blended)
			drawList->AddRect(pos, size, IM_COL32(255, 255, 255, 70));
			drawList->AddRectFilled(pos, size, IM_COL32(255, 255, 255, 20));

			// Basically see if the user clicked a button, but image is button
			if (ImGui::IsMouseClicked(0))
			{
				UpdateCacheDirectoryFiles(file.path);
			}
		}

		ImGui::EndGroup();

		// Draw on same line till newLine is called
		ImGui::SameLine();

		// Draw a new line every amntWide
		if ((i + 1) % amntWide == 0)
		{
			ImGui::NewLine();
		}
	}
}

FileBrowser::~FileBrowser()
{
	
}