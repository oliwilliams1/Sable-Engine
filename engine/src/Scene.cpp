#include "Scene.h"
#include "DebugLog.h"

using namespace SB;

static Scene* s_Instance = nullptr;

Scene::Scene()
{
    m_Nodes.push_back(m_Root);
}

Scene::~Scene() {}

void Scene::Init()
{
    s_Instance = new Scene();
}

void Scene::Shutdown()
{
    delete s_Instance;
    s_Instance = nullptr;
}

Scene& Scene::GetInstance()
{
    return *s_Instance;
}

void Scene::AddNodeToVec(const std::shared_ptr<SceneNode>& node)
{
    m_Nodes.push_back(node);
    DEBUG_LOG("Node added: %s", node->m_Name.c_str());
}

void Scene::AddNode(const std::string& name, const std::shared_ptr<SceneNode>& parentNode)
{
    for (const auto& node : m_Nodes)
    {
        if (node->m_Name == parentNode->m_Name)
        {
            node->AddChild(std::make_shared<SceneNode>(name));
            return;
        }
    }
}

const std::shared_ptr<SceneNode> Scene::GetRoot() const
{
    return m_Root;
}

std::vector<std::shared_ptr<SceneNode>> Scene::GetNodes()
{
    return m_Nodes;
}

void SceneNode::AddChild(std::shared_ptr<SceneNode> child)
{
    m_Children.push_back(child);
    Scene::GetInstance().AddNodeToVec(child);
}

SceneNode::SceneNode(const std::string& name)
{
    m_Name = name;
}