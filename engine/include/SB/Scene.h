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

		std::vector<SceneNode*> m_Children;

	private:
		std::string m_Name;
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

	private:
		Scene();
		~Scene();

		std::string GenerateUniqueName(const std::string& name);

		std::vector<SceneNode*> m_Nodes;
		SceneNode* m_RootNode;
	};
}