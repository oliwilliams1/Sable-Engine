#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class App
{
public:
	App();
	void Mainloop();
	~App();

private:
	GLFWwindow* window;

	int width, height;
	void InitWindow();
};