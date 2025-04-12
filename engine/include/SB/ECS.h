#pragma once
#include <vector>
#include "Mesh.h"
#include "Entity.h"

namespace SB
{
	class ECS
	{
	public:
		ECS(const ECS&) = delete;
		ECS & operator=(const ECS&) = delete;

		static void Init();
		static void Shutdown();
		static ECS& GetInstance();

		Entity CreateEntity(uint32_t parentID = -1);
		void MoveEntity(Entity entity, Entity parent);
		uint32_t GetRootNodeID() const;
	private:
		ECS();
		~ECS();

		uint32_t nextEntityID = 0;
		uint32_t m_RootNodeID = -1;

		std::vector<Entity> m_Entities;
		std::vector<Mesh> m_Meshes;

	};
}