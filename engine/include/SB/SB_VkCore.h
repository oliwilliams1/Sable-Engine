#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <vector>
#include <array>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME "VK_KHR_portability_subset"

// Huge thanks to https://vulkan-tutorial.com/
// And for vulkan imgui setup, https://frguthmann.github.io/posts/vulkan_imgui/
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

	struct ImGuiInitInfo
	{
		VkInstance instance;
		VkPhysicalDevice physicalDevice;
		VkDevice device;
		VkQueue graphicsQueue;
		VkDescriptorPool descriptorPool;
		uint32_t imageCount;
	};

	struct ImGuiImageData
	{
		VkDescriptorSet descriptorSet;
		VkImage image;
		VkImageView imageView;
		VkSampler sampler;
		VkDeviceMemory imageMemory;

		ImGuiImageData() { memset(this, 0, sizeof(*this)); } // saw on imgui doc so its prob cool
	};

	struct FrameAttachment
	{
		VkImage image;
		VkImageView imageView;
		VkFramebuffer framebuffer;
		VkDeviceMemory imageMemory;
		uint32_t width;
		uint32_t height;

		// for imgui
		VkDescriptorSet descriptorSet;
		VkSampler sampler;
	};

	struct VulkanFrame
	{
		uint32_t width, height;
		VkRenderPass renderPass;
		std::vector<FrameAttachment> attachments;
		std::vector<VkCommandBuffer> commandBuffers;
	};

	class VkCore
	{
	public:
		VkCore(const VkCore&) = delete;
		VkCore& operator=(const VkCore&) = delete;

		static void Init();
		static void Shutdown();
		static VkCore& Get();
		void InitVk(uint32_t glfwExtensionCount, const char** glfwExtensions);

		void GetImGuiInitInfo(ImGuiInitInfo& info);
		VkDescriptorPool MakeDescriptorPool(VkDescriptorPoolSize* poolSizes, uint32_t poolSizeCount);

		void CreateSwapchainTexture(VkFormat format, VulkanFrame& frame);

		void SetFramebufferSize(int width, int height);

		void BeginFrame();
		void Draw();
		void EndFrame();

		void ResizeMainFrame(int width, int height);

		void LoadTexture(const std::string& filename, ImGuiImageData& texture);

		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

		VkCommandBuffer GetCurrentCommandBuffer() { return m_commandBuffers[currentFrame]; }

		VkRenderPass MakeImGuiRenderPass();

		VkDevice GetDevice() const { return device; }

		using FuncPtr = void(*)(VkInstance, VkSurfaceKHR*);
		void AttachCreateSurfaceFunction(FuncPtr func);

		void RecreateSwapchain();

		void CreateTextureSampler(VkSampler* sampler);

		VulkanFrame MainFrame;
		std::vector<VkSampler> samplers;
		VkDescriptorSet GetViewportTexture() { return MainFrame.attachments[imageIndex].descriptorSet; }

	private:
		VkCore() {};
		~VkCore();


		uint32_t imageIndex = 0;

		const std::vector<Vertex> vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0
		};

		struct UniformBufferObject {
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		};

		int m_FramebufferWidth, m_FramebufferHeight = 0;

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		VkInstance vkInstance;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		VkRenderPass m_renderPass;
		VkQueue graphicsQueue;
		uint32_t graphicsQueueFamilyIndex;
		std::vector<VkImage> swapChainImages;
		VkCommandPool commandPool;

		std::vector<VkRenderPass> renderPasses;

		VkDebugUtilsMessengerEXT debugMessenger;
		VkSurfaceKHR surface;

		VkQueue presentQueue;

		VkSwapchainKHR swapchain;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

		std::vector<VkImageView> swapChainImageViews;

		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::vector<VkCommandBuffer> m_commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;

		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorPool> descriptorPools;
		std::vector<VkDescriptorSet> descriptorSets;

		VkImage textureImage;
		VkImageView textureImageView;
		VkSampler textureSampler;
		VkDeviceMemory textureImageMemory;
		std::vector<ImGuiImageData> images;

		bool framebufferResized = false;

		uint32_t currentFrame = 0;

		const int MAX_FRAMES_IN_FLIGHT = 2;

		std::vector<const char*> m_Extensions;
		
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation",
		};

		std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
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
		void createRenderPass(VkFormat swapChainImageFormat, VkRenderPass& renderPass, VkImageLayout imageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

		void createGraphicsPipeline();
		VkShaderModule createShaderModule(const std::vector<char>& code);

		void createFramebuffers();
		void createCommandPool();
		void createCommandBuffers(std::vector<VkCommandBuffer>& commandBuffers);

		void createSyncObjects();

		void cleanupSwapchain();

		void createVertexBuffer();
		void createIndexBuffer();
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		void createDescriptorSetLayout();
		void createUniformBuffers();
		void updateUniformBuffer(uint32_t currentImage);

		void createDescriptorPool();
		void createDescriptorSets();

		void createTextureImage(const std::string& path, VkImage& textureImage, VkDeviceMemory& textureImageMemory);
		void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		void createTextureImageView();
		void createImageView(VkImage& image, VkImageView& imageView, VkFormat format);

		void createFramebuffer(FrameAttachment& frameAttachments);

		bool checkPhysicalDeviceSupportsPortability(VkPhysicalDevice device);
	};
}
