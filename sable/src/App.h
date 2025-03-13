#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <SB/SableEngine.h>
#include <SB/SB_VkCore.h>

class App
{
public:
	App();
	~App();

	void Run();
	void ReiszePublicCallback();

private:
	inline static GLFWwindow* window = nullptr;

	SB::VkCore* vkCore = nullptr;

	int width, height;
	void InitWindow();

	static void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
};