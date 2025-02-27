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

		void Draw();

		using FuncPtr = void(*)(VkInstance, VkSurfaceKHR*);
		void AttachCreateSurfaceFunction(FuncPtr func);

		void RecreateSwapchain();

		VkDescriptorPool CreateDiscriptorPool(uint32_t maxSets);

		VkInstance vkInstance;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		VkRenderPass renderPass;
		VkQueue graphicsQueue;
		uint32_t graphicsQueueFamilyIndex;
		std::vector<VkImage> swapChainImages;
		VkCommandPool commandPool;

	private:
		std::vector<VkDescriptorPool> m_DescriptorPools;

		int m_FramebufferWidth, m_FramebufferHeight = 0;

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else 
		const bool enableValidationLayers = true;
#endif

		VkDebugUtilsMessengerEXT debugMessenger;
		VkSurfaceKHR surface;

		VkQueue presentQueue;

		VkSwapchainKHR swapchain;
		VkFormat swapChainImageFormat;
		VkExtent2D m_SwapChainExtent;

		std::vector<VkImageView> swapChainImageViews;

		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		bool framebufferResized = false;

		uint32_t currentFrame = 0;

		const int MAX_FRAMES_IN_FLIGHT = 2;

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
		
		void createSwapchain();
		void createImageViews();
		void createRenderPass();
		
		void createGraphicsPipeline();
		VkShaderModule createShaderModule(const std::vector<char>& code);

		void createFramebuffers();
		void createCommandPool();
		void createCommandBuffers();
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

		void createSyncObjects();

		void cleanupSwapchain();
	};
}