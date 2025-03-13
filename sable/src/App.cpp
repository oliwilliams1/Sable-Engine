#include "App.h"
#include <SB/DebugLog.h>

static void error_callback(int error, const char* description)
{
	if (error == 65548)
	{
		// Error is a dumb wayland error, which i do not care about, already fixed with macros
		return;
	}
	SB::SABLE_WARN("GLFW::ERROR_CALLBACK: %i: %s", error, description);
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
	app->ReiszePublicCallback();
}

App::App()
{
	width = 1600;
	height = 900;
	InitWindow();

	SBEngine::Init();
}

void App::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
	{
		SB::SABLE_ERROR("Failed to create window surface!");
		throw std::runtime_error("Failed to create window surface!");
	}
	SB::SABLE_LOG("Window surface created suceesfully");
}

void App::InitWindow()
{
	if (!glfwInit())
	{
		SB::SABLE_ERROR("Failed to initialize GLFW");
		exit(1);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(width, height, "Sable Editor", nullptr, nullptr);

	if (!window)
	{
		SB::SABLE_ERROR("Failed to create GLFW window");
		glfwTerminate();
		exit(1);
	}

	glfwSetErrorCallback(error_callback);

	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	glfwSetWindowSizeLimits(window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	int fbW, fbH;
	glfwGetFramebufferSize(window, &fbW, &fbH);

	SB::VkCore::Init();
	vkCore = &SB::VkCore::Get();
	vkCore->AttachCreateSurfaceFunction(CreateWindowSurface);
	vkCore->SetFramebufferSize(fbW, fbH);
	vkCore->InitVk(glfwExtensionCount, glfwExtensions);

	SB::SABLE_LOG("GLFW window created for Vulkan");
}

void App::ReiszePublicCallback()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);

	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkCore->SetFramebufferSize(width, height);
	vkCore->RecreateSwapchain();
}

void App::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		vkCore->BeginFrame();
		vkCore->Draw();
		vkCore->EndFrame();
	}
}

App::~App()
{
	vkDeviceWaitIdle(vkCore->GetDevice());

	vkCore->Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();
	SBEngine::Shutdown();
}