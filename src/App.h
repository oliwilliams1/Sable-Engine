#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "FileBrowser.h"
#include "SB/SableEngine.h"

class App 
{
public:
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	static void Init();
	static void Shutdown();

	static App& GetInstance();

	void Mainloop();

private:
	App();
	~App();

	GLFWwindow* window;

	FileBrowser* fileBrowser;

	int width, height;
	void InitWindow();
	void InitImGui();
	void DisplayMenuBar();
	void LoadProject(const std::string& path);

	bool assetManagerWindowOpen = true, consoleWindowOpen = true;

	bool newProjectWindowOpen = false;
};
