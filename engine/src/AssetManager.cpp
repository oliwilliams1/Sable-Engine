#include "DebugLog.h"
#include "AssetManager.h"
#include <iostream>
#include "Utils.h"

#include <stb_image.h>

using namespace SB;

static AssetManager* s_Instance = nullptr;

Mesh* AssetManager::GetMesh(const std::string& name)
{
	auto it = ms_MeshMap.find(name);
	if (it != ms_MeshMap.end())
	{
		return it->second;
	}
	
	ms_MeshMap[name] = MeshArena::AddMesh();
	return ms_MeshMap[name];
}

AssetManager::AssetManager()
{
	MeshArena::Init();
	ms_ResourcePath = GetRelPath("resources");
	SABLE_LOG("Asset manager loaded for path: %s", ms_ResourcePath.string().c_str());
}

AssetManager::~AssetManager()
{
	for (const auto& pair : ms_TextureMap)
	{
		// glDeleteTextures(1, &pair.second.id);
	}

	ms_TextureMap.clear();
	SABLE_LOG("Asset manager destroyed");

	MeshArena::Destroy();
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