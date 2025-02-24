#pragma once

#include <vulkan/vulkan.h>

namespace SB
{
	class VkCore
	{
	public:
		void InitVulkan(uint32_t glfwExtensionCount, const char** glfwExtensions);

		void ShutdownVulkan() const;

	private:
		VkInstance m_VkInstance;
		void createInstance(uint32_t glfwExtensionCount, const char** glfwExtensions);
	};
}