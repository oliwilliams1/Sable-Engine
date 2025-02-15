#include "Scene.h"
#include "DebugLog.h"
#include <algorithm>
#include <iomanip>
#include <sstream>

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

SceneNode* SB::Scene::GetRootNode()
{
    return m_RootNode;
}

void SceneNode::AddChild(SceneNode* child)
{
    if (child) {
        m_Children.push_back(child);
    }
}

SceneNode::SceneNode(const std::string& name) : m_Name(name) {}

const std::string& SceneNode::GetName() const
{
    return m_Name;
}