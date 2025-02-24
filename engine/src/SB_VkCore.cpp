#include <iostream>
#include "SB_VkCore.h"
#include "DebugLog.h"

using namespace SB;

void VkCore::InitVulkan(uint32_t glfwExtensionCount, const char** glfwExtensions)
{
	createInstance(glfwExtensionCount, glfwExtensions);
}

void VkCore::createInstance(uint32_t glfwExtensionCount, const char** glfwExtensions)
{
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

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_VkInstance);

    if (vkCreateInstance(&createInfo, nullptr, &m_VkInstance) != VK_SUCCESS)
    {
        DEBUG_ERROR("Failed to create Vulkan instance!");
        exit(1);
    }
    else
    {
        DEBUG_LOG("Vulkan instance created!");
    }

    /* uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    DEBUG_LOG("Available extensions: ");
    for (const auto& extension : extensions)
    {
        DEBUG_LOG("\t%s", extension.extensionName);
    } */
}

void VkCore::ShutdownVulkan() const
{
    vkDestroyInstance(m_VkInstance, nullptr);
}