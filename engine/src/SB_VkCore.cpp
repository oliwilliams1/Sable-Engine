#include <iostream>
#include <stdexcept>
#include <cstring>
#include <optional>
#include <set>

#include "SB_VkCore.h"
#include "DebugLog.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		SB::SABLE_WARN("[VK debug callback] validation layer: %s", pCallbackData->pMessage);
	}

	return VK_FALSE;
}

static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

using namespace SB;

QueueFamilyIndices VkCore::findQueueFamilies(VkPhysicalDevice device) const
{
	QueueFamilyIndices indices{};

	std::optional<uint32_t> graphicsFamily;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete())
		{
			break;
		}

		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		i++;
	}
	return indices;
}

bool VkCore::isDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device);

	return indices.graphicsFamily.has_value();
}

void VkCore::InitVk(uint32_t glfwExtensionCount, const char** glfwExtensions)
{
	createInstance(glfwExtensionCount, glfwExtensions);
	setupDebugMessenger();

	if (m_CreateSurfaceFuncPtr == nullptr)
	{
		SABLE_ERROR("No create surface function set!");
	}
	else
	{
		m_CreateSurfaceFuncPtr(m_VkInstance, &m_Surface);
	}

	pickPhysicalDevice();
	createLogicalDevice();
}

void VkCore::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(m_PhysicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = 0;

	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
		createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS)
	{
		SABLE_ERROR("Failed to create logical device!");
		throw std::runtime_error("Failed to create logical device!");
	}

	vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
	vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);
}

void VkCore::pickPhysicalDevice()
{
	m_PhysicalDevice = VK_NULL_HANDLE;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		SABLE_ERROR("Nah, youve got no GPUs with vulkan support?!");
		throw std::runtime_error("Nah, youve got no GPUs with vulkan support?!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			m_PhysicalDevice = device;
			break;
		}
	}

	if (m_PhysicalDevice == VK_NULL_HANDLE)
	{
		SABLE_ERROR("Failed to find a suitable GPU!");
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
	else
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &deviceProperties);

		SABLE_LOG("Selected GPU: %s", deviceProperties.deviceName);
	}
}

void VkCore::createInstance(uint32_t glfwExtensionCount, const char** glfwExtensions, bool displayExtensions)
{
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
        SABLE_ERROR("Validation layers requested, but not available!");
        exit(1);
    }

    m_Extensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
		m_Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	bool portabillityEnumerationSupport = checkPortabilityEnumerationSupport();

	if (portabillityEnumerationSupport)
	{
		m_Extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	} 
	else
	{
		SABLE_WARN("Vk Portability enumeration extension unavailable, vulkan instance may have problems initializing");
	}

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Sable Editor";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

	if (portabillityEnumerationSupport)
	{
		createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
	}

    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_Extensions.size());
    createInfo.ppEnabledExtensionNames = m_Extensions.data();

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
        createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers)
    {
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_VkInstance) != VK_SUCCESS) {
		SABLE_ERROR("failed to create vulkan instance! :cry:");
        throw std::runtime_error("failed to create vulkan instance!");
    }

    if (!displayExtensions) return;

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

    SABLE_LOG("Available extensions: ");
    for (const auto& extension : availableExtensions)
    {
        SABLE_LOG("\t%s", extension.extensionName);
    }
}

bool VkCore::checkPortabilityEnumerationSupport()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    bool portabilityExtSupported = false;

    for (const auto& extension : extensions)
    {
        if (strcmp(extension.extensionName, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) == 0)
        {
            portabilityExtSupported = true;
            break;
        }
    }

    return portabilityExtSupported;
}

bool VkCore::checkValidationLayerSupport()
{
    uint32_t layerCount;
    VkResult result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    if (result != VK_SUCCESS)
	{
        std::cerr << "Failed to enumerate instance layer properties." << std::endl;
        return false;
    }

    std::vector<VkLayerProperties> availableLayers(layerCount);
    result = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    if (result != VK_SUCCESS)
	{
        std::cerr << "Failed to enumerate instance layer properties." << std::endl;
        return false;
    }

    for (const char* layerName : m_ValidationLayers)
	{
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
		{
            if (strcmp(layerName, layerProperties.layerName) == 0)
			{
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
		{
            std::cerr << "Validation layer not found: " << layerName << std::endl;
            return false;
        }
    }

    return true;
}

void VkCore::setupDebugMessenger()
{
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(m_VkInstance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
		SABLE_ERROR("Failed to set up debug messenger!");
	}
}

void VkCore::ShutdownVk()
{
	vkDestroySurfaceKHR(m_VkInstance, m_Surface, nullptr);
	vkDestroyDevice(m_Device, nullptr);
	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(m_VkInstance, m_DebugMessenger, nullptr);
	}

	vkDestroyInstance(m_VkInstance, nullptr);
}

void SB::VkCore::AttachCreateSurfaceFunction(FuncPtr func)
{
	m_CreateSurfaceFuncPtr = func;
}