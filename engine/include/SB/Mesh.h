#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <variant> // cry
#include "SB_VkCore.h"

namespace SB
{
	struct PreMeshData
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<unsigned int> indices;
	};

	class Mesh
	{
	public:
		void AddVertexPositionsData(const std::vector<glm::vec3>& data);
		void AddIndicesData(const std::vector<unsigned int>& data);
		void UploadData();

	private:
		bool m_Uploaded = false;
		PreMeshData* m_TempUploadData = nullptr;

		VkBuffer m_VertexBuffer;
		VkBuffer m_IndexBuffer;
		size_t m_IndicesSize = -1;
	};

	class MeshArena
	{
	public:
		MeshArena(const MeshArena&) = delete;
		MeshArena& operator=(const MeshArena&) = delete;

		static void Init();
		static void Destroy();

		static Mesh* AddMesh();

	private:
		MeshArena();
		~MeshArena();

		static inline std::vector<Mesh> m_Meshes;
	};
}