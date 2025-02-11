#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "filebrowser.h"

class App
{
public:
	App();
	void Mainloop();
	~App();

private:
	GLFWwindow* window;

	FileBrowser fileBrowser;

	int width, height;
	void InitWindow();
	void InitImGui();
};