#include "Object.h"
#include "DebugLog.h"

using namespace SB;

Object::Object() {}

template<typename T>
void Object::SetObjectData(T data, SB_OBJECT_DATA_TYPE dataType)
{
    switch (dataType)
    {
    case SB_OBJECT_DATA_VERTICES:
        if (m_ObjData.vertices.empty())
        {
            DEBUG_WARN("Object of node %s already has vertex data... overwriting", m_ParentNode->GetName().c_str());
        }
        m_ObjData.vertices = data;
        break;

    case SB_OBJECT_DATA_NORMALS:
        if (m_ObjData.normals.empty())
        {
            DEBUG_WARN("Object of node %s already has normal data... overwriting", m_ParentNode->GetName().c_str());
        }
        m_ObjData.normals = data;
        break;

    case SB_OBJECT_DATA_INDICES:
        if (m_ObjData.indices.empty())
        {
            DEBUG_WARN("Object of node %s already has index data... overwriting", m_ParentNode->GetName().c_str());
        }
        m_ObjData.indices = data;
        break;

    case SB_OBJECT_DATA_UVS:
        if (m_ObjData.uvs.empty())
        {
            DEBUG_WARN("Object of node %s already has UV data... overwriting", m_ParentNode->GetName().c_str());
        }
        m_ObjData.uvs = data;
        break;

    case SB_OBJECT_DATA_TANGENTS:
        if (m_ObjData.tangents.empty())
        {
            DEBUG_WARN("Object of node %s already has tangent data... overwriting", m_ParentNode->GetName().c_str());
        }
        m_ObjData.tangents = data;
        break;

    case SB_OBJECT_DATA_BITANGENTS:
        if (m_ObjData.bitangents.empty())
        {
            DEBUG_WARN("Object of node %s already has bitangent data... overwriting", m_ParentNode->GetName().c_str());
        }
        m_ObjData.bitangents = data;
        break;

    default:
        DEBUG_WARN("Unknown data type for object of node %s", m_ParentNode->GetName().c_str());
        break;
    }
}

Object::~Object() {}
