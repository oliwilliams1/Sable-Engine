#include "Entity.h"

using namespace SB;

Entity::Entity(uint32_t id) : m_ID(id) {}

void Entity::RegisterChild(uint32_t child)
{
	m_Children.push_back(child);
};