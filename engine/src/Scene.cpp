#include "Scene.h"

using namespace SB;

Scene::Scene() 
{
    m_Root = std::make_unique<SceneNode>("Root");
}

Scene::~Scene()
{

}

void Scene::AddNode(std::unique_ptr<SceneNode> node) 
{
	m_Nodes.push_back(std::move(node));
}

void Scene::RemoveNode(SceneNode* node) 
{
    auto it = std::remove_if(m_Nodes.begin(), m_Nodes.end(),
        [node](const std::unique_ptr<SceneNode>& n) { return n.get() == node; });
    m_Nodes.erase(it, m_Nodes.end());
}

const SceneNode* Scene::GetRoot() const
{
    return m_Root.get();
}

// Raw pointers may not be best implementation
std::vector<SceneNode*> Scene::GetNodes() 
{
    std::vector<SceneNode*> rawPointers;
    for (const auto& node : m_Nodes) {
        rawPointers.push_back(node.get());
    }
    return rawPointers;
}

void SceneNode::AddChild(std::unique_ptr<SceneNode> child)
{
	m_Children.push_back(std::move(child));
}

SceneNode::SceneNode(const std::string& name)
{
    m_Name = name;
}