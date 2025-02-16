#include "Scene.h"
#include "DebugLog.h"
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace SB;

static Scene* s_Instance = nullptr;

Scene::Scene()
{
    m_RootNode = new SceneNode("root");
    m_Nodes.push_back(m_RootNode);
}

Scene::~Scene()
{
    for (SceneNode* node : m_Nodes) {
        delete node;
    }
}

std::string SB::Scene::GenerateUniqueName(const std::string& name)
{
    for (const SceneNode* node : m_Nodes)
    {
        if (node->GetName() == name)
        {
            int index = 1;
            std::string newName = name + ".001";

            while (true)
            {
                bool nameExists = false;
                for (const SceneNode* node : m_Nodes)
                {
                    if (node->GetName() == newName)
                    {
                        nameExists = true;
                        break;
                    }
                }

                if (!nameExists)
                {
                    return newName;
                }

                index++;
                std::stringstream ss;
                ss << std::setfill('0') << std::setw(3) << index;
                newName = name + "." + ss.str();
            }
        }
    }

    return name;
}

void Scene::Init()
{
    if (!s_Instance) {
        s_Instance = new Scene();
    }
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

SceneNode* Scene::AddNode(const std::string& name, SceneNode* parent)
{
    std::string uniqueName = GenerateUniqueName(name);
    SceneNode* newNode = new SceneNode(uniqueName);
    m_Nodes.push_back(newNode);
    if (parent) {
        parent->AddChild(newNode);
    }
    return newNode;
}

void Scene::MoveNodeToParent(SceneNode* node, SceneNode* parent)
{
    for (SceneNode* oldParent : m_Nodes)
    {
        auto& children = oldParent->m_Children;
        auto it = std::find(children.begin(), children.end(), node);

        if (it != children.end())
        {
            children.erase(it);
            DEBUG_LOG("[SCENE]\tNode: %s removed from parent: %s", node->GetName().c_str(), oldParent->GetName().c_str());
            node->SetParent(nullptr);
            break;
        }
    }

    parent->AddChild(node);
}

SceneNode* SB::Scene::GetRootNode()
{
    return m_RootNode;
}

void SceneNode::AddChild(SceneNode* child)
{
    if (child) {
        DEBUG_LOG("[SCENE]\tChild: %s, added to node: %s", child->GetName().c_str(), m_Name.c_str());
        m_Children.push_back(child);
        child->SetParent(this);
    }
}

SceneNode::SceneNode(const std::string& name) : m_Name(name) {}

const std::string& SceneNode::GetName() const
{
    return m_Name;
}

void SceneNode::SetName(const std::string& name)
{
    m_Name = SB::Scene::GetInstance().GenerateUniqueName(name);
}

const SceneNode* SceneNode::GetParent() const
{
    return m_Parent;
}

void SceneNode::SetParent(SceneNode* parent)
{
    m_Parent = parent;
}
