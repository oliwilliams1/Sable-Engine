#pragma once

#include <string>
#include <vector>
#include "Mesh.h"

namespace SB
{
    class SceneNode
    {
    public:
        explicit SceneNode(const std::string& name);
        ~SceneNode() {}

        void AddChild(SceneNode* child);
        const std::string& GetName() const;
        void SetName(const std::string& name);

        const SceneNode* GetParent() const;
        void SetParent(SceneNode* parent);

        void AddMeshComponent();
        const Mesh* GetMeshComponent() const;

        std::vector<SceneNode*> m_Children;
        
    private:
        std::string m_Name;
        Mesh* m_Mesh = nullptr;
        SceneNode* m_Parent = nullptr;
    };

    class Scene
    {
    public:
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        static void Init();
        static void Shutdown();

        static Scene& GetInstance();

        SceneNode* AddNode(const std::string& name, SceneNode* parent);
        SceneNode* GetRootNode();

        void MoveNodeToParent(SceneNode* node, SceneNode* parent);

        std::string GenerateUniqueName(const std::string& name);
    private:
        Scene();
        ~Scene();

        std::vector<SceneNode*> m_Nodes;
        SceneNode* m_RootNode;
    };
}
