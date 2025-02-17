#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

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

    class Mesh
    {
    public:
        Mesh(const std::string& parentNodeName);
        ~Mesh();
        
        template<typename T>
        void SetObjectData(T data, SB_OBJECT_DATA_TYPE dataType);

    private:
        ObjectData m_ObjData;
        std::string m_ParentNodeName;
    };

    class MeshArena
    {
    public:
        MeshArena(const MeshArena&) = delete;
        MeshArena& operator=(const MeshArena&) = delete;

        static void Init();
        static void Shutdown();

        static MeshArena& GetInstance();

        Mesh* AddMesh(const std::string& parentNodeName);

    private:
        MeshArena();
        ~MeshArena();
        std::vector<Mesh> m_Meshes;
    };
}
