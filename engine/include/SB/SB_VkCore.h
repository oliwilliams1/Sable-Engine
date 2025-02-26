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

	struct SwapChainSupportDetails 
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VkCore
	{
	public:

		void InitVk(uint32_t glfwExtensionCount, const char** glfwExtensions);
		void ShutdownVk();

		void SetFramebufferSize(int width, int height);

		using FuncPtr = void(*)(VkInstance, VkSurfaceKHR*);
		void AttachCreateSurfaceFunction(FuncPtr func);

	private:
		int m_FramebufferWidth, m_FramebufferHeight = 0;

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

		VkSwapchainKHR m_Swapchain;
		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;

		std::vector<VkImageView> m_SwapChainImageViews;

		VkRenderPass m_RenderPass;
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;

		std::vector<const char*> m_Extensions;
		const std::vector<const char*> m_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> m_DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
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
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		
		void createSwapChain();
		void createImageViews();
		void createRenderPass();
		
		void createGraphicsPipeline();
		VkShaderModule createShaderModule(const std::vector<char>& code);
	};
}