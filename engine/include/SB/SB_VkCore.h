#pragma once

#include <vulkan/vulkan.h>
#include <vector>

// Huge thanks to https://vulkan-tutorial.com/
namespace SB
{
	class VkCore
	{
	public:
		void InitVk(uint32_t glfwExtensionCount, const char** glfwExtensions);
		void ShutdownVk();

	private:

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else 
		const bool enableValidationLayers = true;
#endif

		VkInstance m_VkInstance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_Device;
		VkQueue m_GraphicsQueue;

		std::vector<const char*> m_Extensions;
		const std::vector<const char*> m_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		void createInstance(uint32_t glfwExtensionCount, const char** glfwExtensions, bool displayExtensions = false);
		bool checkValidationLayerSupport();
		bool checkPortabilityEnumerationSupport();
		void setupDebugMessenger();
		void pickPhysicalDevice();
		void createLogicalDevice();
	};
}