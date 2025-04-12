#include "ECS.h"
#include "DebugLog.h"

using namespace SB;

static ECS* s_Instance = nullptr;

void ECS::Init()
{
	s_Instance = new ECS();
}

void ECS::Shutdown()
{
	delete s_Instance;
	s_Instance = nullptr;
}

ECS& ECS::GetInstance()
{
	return *s_Instance;
}

ECS::ECS()
{
	SABLE_LOG("ECS initialized");
	m_RootNodeID = CreateEntity().GetID();
}

Entity ECS::CreateEntity(uint32_t parentID)
{
	Entity member(nextEntityID++);
	m_Entities.push_back(member);

	if (parentID != -1)
	{
		if (m_RootNodeID != -1)
		{
			SABLE_WARN("Trying to create an entity with no parent, when root is already initialized");
		}
		m_Entities[parentID].RegisterChild(member.GetID());
	}
	return member;
}

void ECS::MoveEntity(Entity entity, Entity parent)
{
	
}

uint32_t ECS::GetRootNodeID() const
{
	return m_RootNodeID;
}

ECS::~ECS()
{
	SABLE_LOG("ECS destroyed");
};