#include "Scene.h"
#include "DebugLog.h"
#include <algorithm>
#include <iomanip>
#include <sstream>

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
}

void Scene::MoveNodeToParent(const std::shared_ptr<SceneNode>& node, const std::shared_ptr<SceneNode>& parentNode)
{
    if (!node || !parentNode || node == parentNode)
    {
        return;
    }

    // Find the current parent of the node
    auto currentParent = std::find_if(m_Nodes.begin(), m_Nodes.end(),
        [&node](const std::shared_ptr<SceneNode>& n) {
            return std::find(n->m_Children.begin(), n->m_Children.end(), node) != n->m_Children.end();
        });

    // If a current parent was found, remove the node from its children
    if (currentParent != m_Nodes.end())
    {
        (*currentParent)->m_Children.erase(std::remove((*currentParent)->m_Children.begin(),
            (*currentParent)->m_Children.end(), node), (*currentParent)->m_Children.end());
    }

    // Add the node to the new parent's children
    parentNode->AddChild(node);

    DEBUG_LOG("Moved node: %s to parent node: %s", node->m_Name.c_str(), parentNode->m_Name.c_str());
}

void Scene::AddNode(const std::string& name, const std::shared_ptr<SceneNode>& parentNode)
{
    // I could never write this c++ spaghetti, thank you chagpti!
    // Code bascially enforces unique names for nodes, i.e. node.001 if node already exists or node.004 if all previous exists
    auto parentIterator = std::find_if(m_Nodes.begin(), m_Nodes.end(),
        [&parentNode](const std::shared_ptr<SceneNode>& node) {
            return node->m_Name == parentNode->m_Name;
        });

    if (parentIterator != m_Nodes.end())
    {
        std::string uniqueName = name;
        int suffix = 1;

        while (true)
        {
            bool nameExists = false;
            for (const auto& node : m_Nodes)
            {
                if (node->m_Name == uniqueName)
                {
                    nameExists = true;
                    break;
                }
            }

            if (!nameExists)
            {
                break;
            }

            std::ostringstream oss;
            oss << name << "." << std::setw(3) << std::setfill('0') << suffix++;
            uniqueName = oss.str();
        }

        (*parentIterator)->AddChild(std::make_shared<SceneNode>(uniqueName));
    }

    DEBUG_LOG("Node added: %s, parent node: %s", name.c_str(), parentNode->m_Name.c_str());
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