#include <vulkan/vulkan.h>
//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_vulkan.h>
#include "App.h"
#include "Utils.h"
#include "SB/DebugLog.h"
#include "SB/Utils.h"

#include "ConsoleDisplay.h"
#include "ProjectManager.h"


static App* s_AppInstance = nullptr;

static void error_callback(int error, const char* description)
{
    if (error == 65548)
    {
        // Error is a dumb wayland error, which i do not care about, already fixed with macros
        return;
    }
	SB::SABLE_WARN("GLFW::ERROR_CALLBACK: %i: %s", error, description);
}

void App::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
	{
		SB::SABLE_ERROR("Failed to create window surface!");
		throw std::runtime_error("Failed to create window surface!");
	}
	SB::SABLE_LOG("Window surface created suceesfully");
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
				SB::SABLE_LOG("User creating new project, displaying window...");
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

			if (ImGui::MenuItem("ImGui Demo Window"))
			{
				imguiDemoWindowOpen = !imguiDemoWindowOpen;
			}
			ImGui::SameLine(checkboxWidth);
			ImGui::TextUnformatted((imguiDemoWindowOpen ? "[x]" : "[ ]"));

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void App::InitWindow()
{
	if (!glfwInit())
	{
		SB::SABLE_ERROR("Failed to initialize GLFW");
		exit(1);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // disable opengl context
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, "Sable Editor", nullptr, nullptr);

	if (!window)
	{
		SB::SABLE_ERROR("Failed to create GLFW window");
		glfwTerminate();
		exit(1);
	}

	glfwSetErrorCallback(error_callback);

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	int fbW, fbH;
	glfwGetFramebufferSize(window, &fbW, &fbH);

	vkCore.AttachCreateSurfaceFunction(CreateWindowSurface);
	vkCore.SetFramebufferSize(fbW, fbH);
	vkCore.InitVk(glfwExtensionCount, glfwExtensions);

	SB::SABLE_LOG("GLFW window created for Vulkan");
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
		SB::SABLE_ERROR("Failed to load project: %s", projPathStr.c_str());
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
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		vkCore.Draw();
	}

	vkCore.ShutdownVk();
}

App::~App()
{
	glfwDestroyWindow(window);
	glfwTerminate();

	delete fileBrowser;

	SBEngine::Shutdown();
}
