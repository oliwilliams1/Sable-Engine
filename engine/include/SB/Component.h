#pragma once

namespace SB
{
	enum class ComponentType
	{
		MESH = 0,
	};

	class Component
	{
	public:
		Component() {}
		virtual ~Component() = default;

		virtual void Init() {}
		virtual void Update() {}
		virtual void Destroy() {}

	private:
		ComponentType m_Type;
	};
}