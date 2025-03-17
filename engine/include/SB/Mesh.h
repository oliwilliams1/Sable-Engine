#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <type_traits>

namespace SB
{
    enum class SB_OBJECT_DATA_TYPE
    {
        VERTICES = 0,
        NORMALS = 1,
        INDICES = 2,
        UVS = 3,
        TANGENTS = 4,
        BITANGENTS = 5
    };

    enum class SB_MESH_DATA_TYPE
    {
        UNDDEF = 0,
        VN = 1
    };

    struct ObjectData
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec3> bitangents;
        std::string m_ParentNodeName;

        template<typename T>
        void SetObjectData(const std::vector<T>& data, SB_OBJECT_DATA_TYPE dataType);
    };

    struct BasicVertex
    {
        glm::vec3 position;
        glm::vec3 normal;
    };

    struct BasicMesh
    {
        std::vector<BasicVertex> vertices;
        std::vector<unsigned int> indices;
    };

    class Mesh
    {
    public:
        Mesh(const std::string& parentNodeName);
        ~Mesh();
        
        void UploadMesh(ObjectData& objData);

    private:
        BasicMesh m_ObjData;
        SB_MESH_DATA_TYPE m_DataType = SB_MESH_DATA_TYPE::UNDDEF;

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
