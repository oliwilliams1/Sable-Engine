#include <iostream>
#include "app.h"

static void error_callback(int error, const char* description)
{
	std::cout << "GLFW::ERROR_CALLBACK: " << error << ": " << description << std::endl;
}

App::App()
{
	width = 1600;
	height = 900;

	InitWindow();
}

void App::InitWindow()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		exit(1);
	}

	window = glfwCreateWindow(width, height, "Sable Editor", nullptr, nullptr);

	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	}

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	glfwMakeContextCurrent(window);

	glfwSetErrorCallback(error_callback);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
		exit(1);
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
}

void App::Mainloop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
	}
}

App::~App()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}