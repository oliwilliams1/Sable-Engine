#pragma once

#include <vulkan/vulkan.h>
#include <vector>

// Huge thanks to https://vulkan-tutorial.com/
namespace SB
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() const
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	class VkCore
	{
	public:
		void InitVk(uint32_t glfwExtensionCount, const char** glfwExtensions);
		void ShutdownVk();

		using FuncPtr = void(*)(VkInstance, VkSurfaceKHR*);
		void AttachCreateSurfaceFunction(FuncPtr func);

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
		VkSurfaceKHR m_Surface;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		std::vector<const char*> m_Extensions;
		const std::vector<const char*> m_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		FuncPtr m_CreateSurfaceFuncPtr = nullptr;

		void createInstance(uint32_t glfwExtensionCount, const char** glfwExtensions, bool displayExtensions = false);
		bool checkValidationLayerSupport();
		bool checkPortabilityEnumerationSupport();
		void setupDebugMessenger();
		void pickPhysicalDevice();
		void createLogicalDevice();
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
		bool isDeviceSuitable(VkPhysicalDevice device);
	};
}