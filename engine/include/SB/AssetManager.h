#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <memory>

namespace SB
{
	enum SB_ASSET_TYPE
	{
		SBE_ASSET_TEXTURE = 0,
		SBE_ASSET_MESH = 1,
		SBE_ASSET_MATERIAL = 2
	};

	struct SB_TEXTURE
	{
		int width, height, channels;
		// GLuint id = -1;
	};

	struct SB_MESH
	{
		int test;
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

		static bool LoadTextureFromFile(const std::string& filename, SB_TEXTURE& texture);
		static bool LoadMeshFromFile(const std::string& filename, SB_MESH& mesh);

	private:
		AssetManager();
		~AssetManager();
		
		static inline std::filesystem::path ms_ResourcePath;
		// Possible but not thought through
		static inline std::map<std::string, SB_TEXTURE> ms_TextureMap;
		static inline std::map<std::string, SB_MESH> ms_MeshMap;
		static inline std::map<std::string, SB_MATERIAL> ms_MaterialMap;
	};
}