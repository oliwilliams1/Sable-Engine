#include "Mesh.h"
#include "DebugLog.h"

using namespace SB;

void Mesh::AddVertexPositionsData(const std::vector<glm::vec3>& data)
{
	if (m_TempUploadData == nullptr)
	{
		m_TempUploadData = new PreMeshData();
	}

	if (m_Uploaded)
	{
		SABLE_ERROR("Mesh data already uploaded, skipping");
		return;
	}

	if (data.size() == 0)
	{
	SABLE_ERROR("Mesh does not have vertex position data, skipping");
		return;
	}

	if (m_TempUploadData->vertices.size() != 0)
	{
		SABLE_ERROR("Mesh already has vertex position data, skipping");
		return;
	}

	m_TempUploadData->vertices = data;
}

void Mesh::AddIndicesData(const std::vector<unsigned int>& data)
{
	if (m_TempUploadData == nullptr)
	{
		m_TempUploadData = new PreMeshData();
	}

	if (m_Uploaded)
	{
		SABLE_ERROR("Mesh data already uploaded, skipping");
		return;
	}

	if (data.size() == 0)
	{
		SABLE_ERROR("Mesh does not have index data, skipping");
		return;
	}

	if (m_TempUploadData->indices.size() != 0)
	{
		SABLE_ERROR("Mesh already has index data, skipping");
		return;
	}

	m_TempUploadData->indices = data;
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
	flatData.resize(verticesSize * 5);

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

static MeshArena* s_MeshArenaInstance = nullptr;

void MeshArena::Init()
{
	s_MeshArenaInstance = new MeshArena();
}

void MeshArena::Destroy()
{
	delete s_MeshArenaInstance;
	s_MeshArenaInstance = nullptr;
}

MeshArena::MeshArena() {};
MeshArena::~MeshArena() {};

Mesh* MeshArena::AddMesh()
{
	m_Meshes.emplace_back();
	return &m_Meshes.back();
}
