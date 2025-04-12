#pragma once
#include <chrono>

#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include "FileBrowser.h"
#include "SB/SableEngine.h"
#include "SB/ProjectManager.h"
#include "SB/SB_VkCore.h"

class App 
{
public:
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	static void Init();
	static void Shutdown();

	static App& GetInstance();

	void Mainloop();
	void LoadProject(const std::string& path);
	SB::SB_Project GetCurrentProject();

	void ReiszePublicCallback();

	
private:
	App();
	~App();

	inline static GLFWwindow* window = nullptr;

	FileBrowser* fileBrowser = nullptr;

	SB::SB_Project currentProject;
	SB::VkCore* vkCore = nullptr;

	VkCommandBuffer imguiCommandBuffer;

	int width, height;
	void InitWindow();
	void InitImGui();
	void DisplayMenuBar();
	
	static void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	bool assetManagerWindowOpen = true, consoleWindowOpen = true, sceneHeirachyWindowOpen = true;
	bool imguiDemoWindowOpen = false;
	bool newProjectWindowOpen = false;

	int frameCount;
	double averageFPS;
	std::chrono::high_resolution_clock::time_point lastTime;
};
