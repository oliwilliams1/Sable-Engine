#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <glm/glm.hpp>

// Huge thanks to https://vulkan-tutorial.com/
namespace SB
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 colour;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			/*float: VK_FORMAT_R32_SFLOAT
			  vec2: VK_FORMAT_R32G32_SFLOAT
			  vec3 : VK_FORMAT_R32G32B32_SFLOAT
			  vec4 : VK_FORMAT_R32G32B32A32_SFLOAT*/


			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, colour);

			return attributeDescriptions;
		}
	};

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

		VkInstance vkInstance;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		VkRenderPass renderPass;
		VkQueue graphicsQueue;
		uint32_t graphicsQueueFamilyIndex;
		std::vector<VkImage> swapChainImages;
		VkCommandPool commandPool;

	private:
		const std::vector<Vertex> vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0
		};

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
		VkExtent2D swapChainExtent;

		std::vector<VkImageView> swapChainImageViews;

		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

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

		void createVertexBuffer();
		void createIndexBuffer();
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	};
}