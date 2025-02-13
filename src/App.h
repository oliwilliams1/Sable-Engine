#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "FileBrowser.h"
#include "SceneViewer.h"
#include "SB/SableEngine.h"
#include "SB/ProjectManager.h"

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

	GLFWwindow* window;

	FileBrowser* fileBrowser = nullptr;
	SceneViewer* sceneViewer = nullptr;

	SB::SB_Project currentProject;

	int width, height;
	void InitWindow();
	void InitImGui();
	void DisplayMenuBar();

	bool assetManagerWindowOpen = true, consoleWindowOpen = true, sceneHeirachyWindowOpen = true;
	bool imguiDemoWindowOpen = false;
	bool newProjectWindowOpen = false;
};
