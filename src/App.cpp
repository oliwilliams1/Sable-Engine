#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "App.h"
#include "Utils.h"
#include "SB/DebugLog.h"
#include "SB/Utils.h"
#include "ConsoleDisplay.h"
#include "ProjectManager.h"

static App* s_AppInstance = nullptr;

static void error_callback(int error, const char* description)
{
	SB::DEBUG_WARN("GLFW::ERROR_CALLBACK: %i: %s", error, description);
}

void App::Init()
{
	s_AppInstance = new App();
}

void App::Shutdown()
{
	delete s_AppInstance;
	s_AppInstance = nullptr;
}

App& App::GetInstance()
{
	return *s_AppInstance;
}

App::App()
{
	width = 1600;
	height = 900;
	InitWindow();
	InitImGui();
	SetupImGuiStyle();

	SBEngine::Init();

	currentProject = SB::SB_Project();

	fileBrowser = new FileBrowser();
	sceneViewer = new SceneViewer();
}

void App::DisplayMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			const float keybindHintWidth = 120.0f;

			if (ImGui::MenuItem("New Project")) 
			{
				SB::DEBUG_LOG("User creating new project, displaying window...");
				newProjectWindowOpen = true;
			}
			ImGui::SameLine(keybindHintWidth);
			ImGui::TextUnformatted("CTRL+N");

			if (ImGui::MenuItem("Open Project...")) {}
			ImGui::SameLine(keybindHintWidth);
			ImGui::TextUnformatted("CTRL+O");

			if (ImGui::MenuItem("Save Project")) {}
			ImGui::SameLine(keybindHintWidth);
			ImGui::TextUnformatted("CTRL+S");

			if (ImGui::MenuItem("Save Project As...")) {}

			ImGui::SameLine(keybindHintWidth);
			ImGui::TextUnformatted("CTRL+SHIFT+S");

			ImGui::Separator();

			if (ImGui::MenuItem("Import Asset...")) {}

			if (ImGui::MenuItem("Build Asset Pack")) {}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
			{
				glfwSetWindowShouldClose(window, true);
			}
			ImGui::SameLine(keybindHintWidth);
			ImGui::TextUnformatted("CTRL+Q");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("haha nothing here");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			const float checkboxWidth = 120.0f;
			if (ImGui::MenuItem("Asset manager"))
			{
				assetManagerWindowOpen = !assetManagerWindowOpen;
			}
			ImGui::SameLine(checkboxWidth);
			ImGui::TextUnformatted((assetManagerWindowOpen ? "[x]" : "[ ]"));

			if (ImGui::MenuItem("Console"))
			{
				consoleWindowOpen = !consoleWindowOpen;
			}
			ImGui::SameLine(checkboxWidth);
			ImGui::TextUnformatted((consoleWindowOpen ? "[x]" : "[ ]"));

			if (ImGui::MenuItem("Scene Hierarchy"))
			{
				sceneHeirachyWindowOpen = !sceneHeirachyWindowOpen;
			}
			ImGui::SameLine(checkboxWidth);
			ImGui::TextUnformatted((sceneHeirachyWindowOpen ? "[x]" : "[ ]"));
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void App::InitWindow()
{
	if (!glfwInit())
	{
		SB::DEBUG_ERROR("Failed to initialize GLFW");
		exit(1);
	}

	window = glfwCreateWindow(width, height, "Sable Editor", nullptr, nullptr);

	if (!window)
	{
		SB::DEBUG_ERROR("Failed to create GLFW window");
		glfwTerminate();
		exit(1);
	}

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	glfwMakeContextCurrent(window);

	glfwSetErrorCallback(error_callback);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		SB::DEBUG_ERROR("Failed to initialize GLEW: %s", glewGetErrorString(err));
	}

	SB::DEBUG_LOG("OpenGL version: %s", glGetString(GL_VERSION));
}

void App::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Detatched windows do not play well with experimental window managers (unix has a billion)
#if defined(UNIX)
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#else
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
#endif

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void App::LoadProject(const std::string& path)
{
	const std::string extension = ".sbproj";
	std::string projPathStr = path;

	if (projPathStr.size() >= extension.size() &&
		projPathStr.compare(projPathStr.size() - extension.size(), extension.size(), extension) == 0)
	{
		projPathStr = projPathStr.substr(0, projPathStr.size() - extension.size());
	}

	SB::SB_Project project;
	if (!EditorProjManager::LoadProject(projPathStr, project))
	{
		SB::DEBUG_ERROR("Failed to load project: %s", projPathStr.c_str());
		return;
	}

	std::filesystem::path relPath = GetRelPath("projects");
	std::filesystem::path projPath = relPath / projPathStr;

	fileBrowser->SetPath(projPath.string());
	fileBrowser->SetCurrentProjectPath(projPath);
	currentProject = project;

	std::string windowTitle = "Sable Engine - " + projPath.filename().string();
	glfwSetWindowTitle(window, windowTitle.c_str());
}

SB::SB_Project App::GetCurrentProject()
{
	return currentProject;
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

		DisplayMenuBar();

		ImGui::DockSpaceOverViewport();

		ImGui::Begin("Docking");
		ImGui::Text("Hello!");
		ImGui::End();

		if (consoleWindowOpen)
		{
			SB_EditorConsole::DisplayConsole();
		}

		if (assetManagerWindowOpen)
		{
			fileBrowser->Render();
		}

		if (sceneHeirachyWindowOpen)
		{
			sceneViewer->Draw();
		}

		if (newProjectWindowOpen)
		{
			SB::SB_Project project;
			if (EditorProjManager::DisplayNewProjectWindow(&newProjectWindowOpen, project))
			{
				LoadProject(project.name);
			}
		}

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

	delete fileBrowser;

	SBEngine::Shutdown();
}
