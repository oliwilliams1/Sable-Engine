#pragma once

#include <string>
#include <vector>
#include <memory>

namespace SB
{
	class SceneNode
	{
	public:
		explicit SceneNode(const std::string& name);
		~SceneNode() = default;

		void AddChild(std::shared_ptr<SceneNode> child);
		std::string m_Name;

		std::vector<std::shared_ptr<SceneNode>> m_Children;

	private:
		// std::shared_ptr<Entity> m_Entity; // Uncomment if needed
	};

	class Scene
	{
	public:
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

		static void Init();
		static void Shutdown();

		static Scene& GetInstance();

		void AddNode(const std::string& name, const std::shared_ptr<SceneNode>& parentNode);
		void AddNodeToVec(const std::shared_ptr<SceneNode>& node);

		void MoveNodeToParent(const std::shared_ptr<SceneNode>& node, const std::shared_ptr<SceneNode>& parentNode);

		std::vector<std::shared_ptr<SceneNode>> GetNodes();
		const std::shared_ptr<SceneNode> GetRoot() const;

	private:
		Scene();
		~Scene();
		std::vector<std::shared_ptr<SceneNode>> m_Nodes;
		std::shared_ptr<SceneNode> m_Root = std::make_shared<SceneNode>("root");
	};
}