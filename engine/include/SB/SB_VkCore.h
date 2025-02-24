#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace SB
{
	class VkCore
	{
	public:
		void InitVulkan(uint32_t glfwExtensionCount, const char** glfwExtensions);
		void ShutdownVulkan();

	private:

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else 
		const bool enableValidationLayers = true;
#endif

		VkInstance m_VkInstance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		std::vector<const char*> m_Extensions;
		const std::vector<const char*> m_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		void createInstance(uint32_t glfwExtensionCount, const char** glfwExtensions, bool displayExtensions = false);
		bool checkValidationLayerSupport();
		void setupDebugMessenger();
		void pickPhysicalDevice() const;
	};
}