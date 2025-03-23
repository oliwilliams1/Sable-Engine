#include "Mesh.h"
#include "DebugLog.h"

using namespace SB;

template<typename T>
void Mesh::AddData(const T& data, VERTEX_DATA_TYPE type)
{
	static_assert(std::is_same_v<T, std::vector<glm::vec3>> ||
		std::is_same_v<T, std::vector<glm::vec2>> ||
		std::is_same_v<T, std::vector<float>> ||
		std::is_same_v<T, std::vector<unsigned int>>,
		"Type must be std::vector<glm::vec3>, std::vector<glm::vec2>, std::vector<float>, or std::vector<unsigned int>");

	if (m_Uploaded)
	{
		SABLE_WARN("Mesh data already uploaded, skipping");
		return;
	}

	if (!m_TempUploadData)
	{
		m_TempUploadData = new PreMeshData();
	}

	switch (type)
	{
	case VERTEX_DATA_TYPE::POSITION:
	{
		if (!std::is_same_v<T, std::vector<glm::vec3>>)
		{
			SABLE_RUNTIME_ERROR("Position data must be of type std::vector<glm::vec3>");
			return;
		}

		m_TempUploadData->vertices = data;
		break;
	}

	case VERTEX_DATA_TYPE::INDICES:
	{
		if (!std::is_same_v<T, std::vector<unsigned int>>)
		{
			SABLE_RUNTIME_ERROR("Indices data must be of type std::vector<unsigned int>");
			return;
		}

		m_TempUploadData->indices = data;
		break;
	}

	default:
		SABLE_RUNTIME_ERROR("Unknown VERTEX_DATA_TYPE");
		return;
	}
}

void Mesh::UploadData()
{
	if (m_Uploaded)
	{
		SABLE_WARN("Mesh data already uploaded, skipping");
		return;
	}

	if (!m_TempUploadData)
	{
		SABLE_WARN("No data to upload for mesh, skipping");
		return;
	}

	size_t indicesSize = m_TempUploadData->indices.size();
	bool hasIndices = indicesSize != 0;

	if (!hasIndices)
	{
		SABLE_ERROR("Mesh does not have index data, skipping");
		return;
	}

	m_IndicesSize = indicesSize;

	size_t verticesSize = m_TempUploadData->vertices.size();
	size_t uvsSize = m_TempUploadData->uvs.size();

	bool hasVertices = verticesSize != 0;
	bool hasUVs = uvsSize != 0;

	if (!hasVertices)
	{
		SABLE_ERROR("Mesh does not have vertex data, skipping");
		return;
	}

	std::vector<float> flatData;
	flatData.reserve(verticesSize * 5);

	for (size_t i = 0; i < verticesSize; i++)
	{
		flatData[i * 5 + 0] = m_TempUploadData->vertices[i].x;
		flatData[i * 5 + 1] = m_TempUploadData->vertices[i].y;
		flatData[i * 5 + 2] = m_TempUploadData->vertices[i].z;

		flatData[i * 5 + 3] = hasUVs ? m_TempUploadData->uvs[i].x : 0.0f;
		flatData[i * 5 + 4] = hasUVs ? m_TempUploadData->uvs[i].y : 0.0f;
	}

	delete m_TempUploadData;
	m_TempUploadData = nullptr;
	m_Uploaded = true;
}

Mesh& MeshArena::AddMesh()
{
	m_Meshes.emplace_back();
	return m_Meshes.back();
}
