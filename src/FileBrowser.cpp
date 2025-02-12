#include <iostream>
#include <imgui.h>
#include "FileBrowser.h"
#include "SableEngine.h"

FileBrowser::FileBrowser()
{
	std::filesystem::path exec_path = std::filesystem::current_path().parent_path();
	std::filesystem::path other_path("resources/");

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

	SBEngine::GetInstance().LoadTextureFromFile("icons/sb-icon-folder.png", m_FolderTexture);
	SBEngine::GetInstance().LoadTextureFromFile("icons/sb-icon-image.png", m_ImageFileTexture);
	SBEngine::GetInstance().LoadTextureFromFile("icons/sb-icon-unknown.png", m_UnknownFileTexture);
	
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
	m_Files.push_back({ "..", m_CurrentPath.parent_path(), SB_ASSET_FOLDER, GetFileTexture(SB_ASSET_FOLDER) }); // Add parent dir button at start of vector
	m_Files.insert(m_Files.end(), p_Directories.begin(), p_Directories.end());
	m_Files.insert(m_Files.end(), p_Files.begin(), p_Files.end());
}

void FileBrowser::RenderDirectory(const File& file)
{
	
}

void FileBrowser::Render()
{
	ImGui::Text("%s", m_CurrentPath.string().c_str());

	ImVec2 window_size = ImGui::GetWindowSize();
	int width = (int)window_size.x;

	int padding = ImGui::GetStyle().ItemSpacing.x;
	int fileWidth = 125;
	int fileHeight = 150;
	int amntWide = (width - padding) / (fileWidth + padding);

	if (amntWide == 0)
	{
		amntWide = 1;
	}

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	float pady = ImGui::GetStyle().ItemSpacing.y;

	for (int i = 0; i < m_Files.size(); ++i)
	{
		const File& file = m_Files[i];

		ImGui::BeginGroup();

		ImGui::Image((ImTextureID)(intptr_t)file.icon, ImVec2(fileWidth, fileWidth));

		ImVec2 pos = ImGui::GetItemRectMin();
		ImVec2 sizeImage = ImGui::GetItemRectSize();

		bool drawHover = false;
		if (ImGui::IsItemHovered())
		{
			drawHover = true;
		}

		ImGui::TextWrapped(file.name.c_str());

		ImVec2 sizeText = ImGui::GetItemRectSize();

		ImVec2 size = ImVec2(pos.x + sizeImage.x, pos.y + sizeImage.y + sizeText.y + pady);

		if (drawHover)
		{
			drawList->AddRect(pos, size, IM_COL32(255, 255, 255, 70));
			drawList->AddRectFilled(pos, size, IM_COL32(255, 255, 255, 20));
		}

		// UpdateCacheDirectoryFiles(file.path);
		
		ImGui::EndGroup();

		ImGui::SameLine();

		if ((i + 1) % amntWide == 0)
		{
			ImGui::NewLine();
		}
	}
}

FileBrowser::~FileBrowser()
{
	
}