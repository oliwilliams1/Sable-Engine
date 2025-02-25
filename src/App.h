#pragma once
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLFW/glfw3.h>
#include "FileBrowser.h"
#include "SceneViewer.h"
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

	
private:
	App();
	~App();

	inline static GLFWwindow* window = nullptr;

	FileBrowser* fileBrowser = nullptr;
	SceneViewer* sceneViewer = nullptr;

	SB::SB_Project currentProject;
	SB::VkCore vkCore;

	int width, height;
	void InitWindow();
	//void InitImGui();
	void DisplayMenuBar();
	
	static void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	bool assetManagerWindowOpen = true, consoleWindowOpen = true, sceneHeirachyWindowOpen = true;
	bool imguiDemoWindowOpen = false;
	bool newProjectWindowOpen = false;
};
