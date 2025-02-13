#include "DebugLog.h"
#include "AssetManager.h"
#include <iostream>
#include "Utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace SB;

static AssetManager* s_Instance = nullptr;

AssetManager::AssetManager()
{
	ms_ResourcePath = GetRelPath("resources");
	DEBUG_LOG("Asset manager loaded for path: %s", ms_ResourcePath.string().c_str());
}

AssetManager::~AssetManager()
{
	for (const auto& pair : ms_TextureMap)
	{
		glDeleteTextures(1, &pair.second.id);
	}

	ms_TextureMap.clear();
	DEBUG_LOG("Asset manager destroyed");
}

void AssetManager::Init()
{
	s_Instance = new AssetManager();
}

void AssetManager::Shutdown()
{
	delete s_Instance;
	s_Instance = nullptr;
}

bool AssetManager::LoadTextureFromFile(const std::string& filename, SB_TEXTURE& texture)
{
	std::filesystem::path path = ms_ResourcePath / filename;

	int width, height, channels;
	unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

	if (data == nullptr)
	{
		DEBUG_ERROR("Failed to load texture: %s", path.string().c_str());
		return false;
	}

	if (texture.id == -1)
	{
		texture.id = 0;
	}
	else
	{
		// Better hope you dont get this error, means texture id already in use
		assert(texture.id != 0);
		return false;
	}

	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	ms_TextureMap[filename] = texture;

	return true;
}

bool AssetManager::LoadMeshFromFile(const std::string& filename, SB_MESH& mesh)
{
	return false;
}