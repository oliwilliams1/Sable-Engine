#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "FileBrowser.h"
#include "SableEngine.h"

class App
{
public:
	App();
	void Mainloop();
	~App();

private:
	GLFWwindow* window;

	FileBrowser* fileBrowser;

	int width, height;
	void InitWindow();
	void InitImGui();
	void DisplayMenuBar();

	bool assetManagerWindowOpen = true, consoleWindowOpen = true;
};