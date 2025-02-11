#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "App.h"
#include "Utils.h"

static void error_callback(int error, const char* description)
{
	std::cout << "GLFW::ERROR_CALLBACK: " << error << ": " << description << std::endl;
}

App::App()
{
	width = 1600;
	height = 900;
	InitWindow();
	InitImGui();
	SetupImGuiStyle();
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
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
}

void App::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

}

void App::Mainloop()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport();

		ImGui::Begin("dokcing");
		ImGui::Text("Helo!");
		ImGui::End();

		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 200), ImVec2(FLT_MAX, FLT_MAX));
		ImGui::Begin("Asset Browser");
		fileBrowser.Render();
		ImGui::End();

		ImGui::Begin("dokcing 2");
		ImGui::Text("Helo!");
		ImGui::End();

		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_window = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_window);
		}

		glfwSwapBuffers(window);
	}
}

App::~App()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
