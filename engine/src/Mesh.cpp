#include "Mesh.h"
#include "DebugLog.h"
#include <type_traits>

using namespace SB;

Mesh::Mesh(const std::string& parentNodeName)
{
    m_ParentNodeName = parentNodeName;
}

template<typename T>
void Mesh::SetObjectData(T data, SB_OBJECT_DATA_TYPE dataType)
{
    static_assert(std::is_same<T, glm::vec3>::value || std::is_same<T, glm::vec2>::value || std::is_same<T, unsigned int>::value,
                "SetObjectData only accepts glm::vec3 glm::vec2, unsigned int");

    switch (dataType)
    {
    case SB_OBJECT_DATA_VERTICES:
        if (m_ObjData.vertices.empty())
        {
            SABLE_WARN("Object of node %s already has vertex data... overwriting", m_ParentNodeName.c_str());
        }
        m_ObjData.vertices = data;
        break;

    case SB_OBJECT_DATA_NORMALS:
        if (m_ObjData.normals.empty())
        {
            SABLE_WARN("Object of node %s already has normal data... overwriting", m_ParentNodeName.c_str());
        }
        m_ObjData.normals = data;
        break;

    case SB_OBJECT_DATA_INDICES:
        if (m_ObjData.indices.empty())
        {
            SABLE_WARN("Object of node %s already has index data... overwriting", m_ParentNodeName.c_str());
        }
        m_ObjData.indices = data;
        break;

    case SB_OBJECT_DATA_UVS:
        if (m_ObjData.uvs.empty())
        {
            SABLE_WARN("Object of node %s already has UV data... overwriting", m_ParentNodeName.c_str());
        }
        m_ObjData.uvs = data;
        break;

    case SB_OBJECT_DATA_TANGENTS:
        if (m_ObjData.tangents.empty())
        {
            SABLE_WARN("Object of node %s already has tangent data... overwriting", m_ParentNodeName.c_str());
        }
        m_ObjData.tangents = data;
        break;

    case SB_OBJECT_DATA_BITANGENTS:
        if (m_ObjData.bitangents.empty())
        {
            SABLE_WARN("Object of node %s already has bitangent data... overwriting", m_ParentNodeName.c_str());
        }
        m_ObjData.bitangents = data;
        break;

    default:
        SABLE_WARN("Unknown data type for object of node %s", m_ParentNodeName.c_str());
        break;
    }
}

Mesh::~Mesh() {}

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