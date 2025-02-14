#pragma once

#include <string>
#include <vector>
#include <memory>

namespace SB
{
    // class Entity;

    class SceneNode 
    {
    public:
        SceneNode(const std::string& name);
        ~SceneNode() = default; // Should be fine for now

        void AddChild(std::unique_ptr<SceneNode> child);
        // void SetEntity(std::shared_ptr<Entity> entity);
        std::string m_Name;

    private:
        std::vector<std::unique_ptr<SceneNode>> m_Children;
        // std::shared_ptr<Entity> m_Entity;
    };

    class Scene 
    {
    public:
        Scene();
        ~Scene();

        void AddNode(std::unique_ptr<SceneNode> node);
        void RemoveNode(SceneNode* node);

        std::vector<SceneNode*> GetNodes();
        const SceneNode* GetRoot() const;

    private:
        std::vector<std::unique_ptr<SceneNode>> m_Nodes;
        std::unique_ptr<SceneNode> m_Root;
    };
}