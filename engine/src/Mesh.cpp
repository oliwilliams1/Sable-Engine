#include "Mesh.h"
#include "DebugLog.h"

using namespace SB;

Mesh::Mesh(const std::string& parentNodeName)
{
    m_ParentNodeName = parentNodeName;
}

template<typename T>
void ObjectData::SetObjectData(const std::vector<T>& data, SB_OBJECT_DATA_TYPE dataType)
{
    // Ensure the template is valid
    static_assert(std::is_same<T, glm::vec3>::value || std::is_same<T, glm::vec2>::value || std::is_same<T, unsigned int>::value,
        "SetObjectData only accepts glm::vec3, glm::vec2, or unsigned int");

    // Set the data based on user data type
    switch (dataType)
    {
    case SB_OBJECT_DATA_TYPE::VERTICES:
        if (!vertices.empty())
        {
            SABLE_WARN("Object of node %s already has vertex data... overwriting", m_ParentNodeName.c_str());
        }
        vertices = data;
        break;

    case SB_OBJECT_DATA_TYPE::NORMALS:
        if (!normals.empty())
        {
            SABLE_WARN("Object of node %s already has normal data... overwriting", m_ParentNodeName.c_str());
        }
        normals = data;
        break;

    case SB_OBJECT_DATA_TYPE::INDICES:
        if (!indices.empty())
        {
            SABLE_WARN("Object of node %s already has index data... overwriting", m_ParentNodeName.c_str());
        }
        indices = data;
        break;

    case SB_OBJECT_DATA_TYPE::UVS:
        if (!uvs.empty())
        {
            SABLE_WARN("Object of node %s already has UV data... overwriting", m_ParentNodeName.c_str());
        }
        uvs = data;
        break;

    case SB_OBJECT_DATA_TYPE::TANGENTS:
        if (!tangents.empty())
        {
            SABLE_WARN("Object of node %s already has tangent data... overwriting", m_ParentNodeName.c_str());
        }
        tangents = data;
        break;

    case SB_OBJECT_DATA_TYPE::BITANGENTS:
        if (!bitangents.empty())
        {
            SABLE_WARN("Object of node %s already has bitangent data... overwriting", m_ParentNodeName.c_str());
        }
        bitangents = data;
        break;

    default:
        SABLE_WARN("Unknown data type for object of node %s", m_ParentNodeName.c_str());
        break;
    }
}


Mesh::~Mesh() {}

void Mesh::UploadMesh(ObjectData& objData)
{
    SB_MESH_DATA_TYPE selectedType = SB_MESH_DATA_TYPE::UNDDEF;

    if (!objData.vertices.empty() && !objData.normals.empty())
    {
        selectedType = SB_MESH_DATA_TYPE::VN;
    }
}

static MeshArena* s_MeshArenaInstance = nullptr;

MeshArena::MeshArena() {};

void MeshArena::Init()
{
    s_MeshArenaInstance = new MeshArena();
}

void MeshArena::Shutdown()
{
	delete s_MeshArenaInstance;
	s_MeshArenaInstance = nullptr;
}

MeshArena& MeshArena::GetInstance()
{
    return *s_MeshArenaInstance;
}

Mesh* MeshArena::AddMesh(const std::string& parentNodeName)
{
    m_Meshes.emplace_back(parentNodeName);
    return &m_Meshes.back();
}

MeshArena::~MeshArena()
{
    m_Meshes.clear();
}