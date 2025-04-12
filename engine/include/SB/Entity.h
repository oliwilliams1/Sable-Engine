#pragma once
#include <vector>
#include <cstdint>

namespace SB
{
	class Entity
	{
	public:
		Entity(uint32_t id);
		~Entity() {}
		uint32_t GetID() const { return m_ID; }
		uint32_t GetParent() { return m_Parent; }

		void RegisterChild(uint32_t child);
		void RegisterParent(uint32_t parent);

		std::vector<uint32_t> m_Children;

	private:
		uint32_t m_Parent;
		uint32_t m_ID;
	};
}