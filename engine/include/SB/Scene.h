#pragma once

#include <string>
#include <vector>

namespace SB
{
	class SceneNode
	{
	public:
		explicit SceneNode(const std::string& name);
		~SceneNode() {}

		void AddChild(SceneNode* child);
		const std::string& GetName() const;
		const SceneNode* GetParent() const;
		void SetParent(SceneNode* parent);

		std::vector<SceneNode*> m_Children;

	private:
		std::string m_Name;
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

	private:
		Scene();
		~Scene();

		std::string GenerateUniqueName(const std::string& name);

		std::vector<SceneNode*> m_Nodes;
		SceneNode* m_RootNode;
	};
}
