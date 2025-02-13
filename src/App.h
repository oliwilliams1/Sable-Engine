#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "FileBrowser.h"
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
	bool GetCurrentProjectLoaded();

private:
	App();
	~App();

	GLFWwindow* window;

	FileBrowser* fileBrowser = nullptr;

	SB::SB_Project currentProject;
	bool projectLoaded = false;

	int width, height;
	void InitWindow();
	void InitImGui();
	void DisplayMenuBar();

	bool assetManagerWindowOpen = true, consoleWindowOpen = true;

	bool newProjectWindowOpen = false;
};
