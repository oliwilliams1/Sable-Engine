#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Scene.h>

namespace SB
{

enum SB_OBJECT_DATA_TYPE
{
    SB_OBJECT_DATA_VERTICES = 0,
    SB_OBJECT_DATA_NORMALS = 1,
    SB_OBJECT_DATA_INDICES = 2,
    SB_OBJECT_DATA_UVS = 3,
    SB_OBJECT_DATA_TANGENTS = 4,
    SB_OBJECT_DATA_BITANGENTS = 5
};

struct ObjectData
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
};

class Object
{
public:
    Object();
    ~Object();
        
    template<typename T>
    void SetObjectData(T data, SB_OBJECT_DATA_TYPE dataType);

private:
    ObjectData m_ObjData;
    SceneNode* m_ParentNode;
};
}
