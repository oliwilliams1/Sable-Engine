#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <memory>
#include "SB_VkCore.h"
#include "Mesh.h"

namespace SB
{
	enum SB_ASSET_TYPE
	{
		SBE_ASSET_TEXTURE = 0,
		SBE_ASSET_MESH = 1,
		SBE_ASSET_MATERIAL = 2
	};

	struct SB_MATERIAL
	{
		int test;
	};

	class AssetManager
	{
	public:
		AssetManager(const AssetManager&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;

		static void Init();
		static void Shutdown();

		static Mesh* GetMesh(const std::string& name);

	private:
		AssetManager();
		~AssetManager();
		
		static inline std::filesystem::path ms_ResourcePath;
		// Possible but not thought through
		static inline std::map<std::string, SB::ImGuiImageData> ms_TextureMap;
		static inline std::map<std::string, Mesh*> ms_MeshMap;
		static inline std::map<std::string, SB_MATERIAL> ms_MaterialMap;
	};
}