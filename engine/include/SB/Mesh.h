#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <variant> // cry
#include "SB_VkCore.h"

namespace SB
{
	enum class VERTEX_DATA_TYPE
	{
		POSITION,
		INDICES
	};

	struct PreMeshData
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<unsigned int> indices;
	};

	class Mesh
	{
	public:
		template<typename T>
		void AddData(const T& data, VERTEX_DATA_TYPE type);
		void UploadData();

	private:
		bool m_Uploaded = false;
		PreMeshData* m_TempUploadData = nullptr;

		VkBuffer m_IndexBuffer;
		size_t m_IndicesSize = -1;
	};

	class MeshArena
	{
	public:
		Mesh& AddMesh();

	private:
		std::vector<Mesh> m_Meshes;
	};
}