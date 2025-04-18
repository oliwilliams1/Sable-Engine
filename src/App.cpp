#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
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

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
	app->ReiszePublicCallback();
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
	InitImGui();

	SBEngine::Init();

	currentProject = SB::SB_Project();

	fileBrowser = new FileBrowser();
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

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(width, height, "Sable Editor", nullptr, nullptr);

	if (!window)
	{
		SB::SABLE_ERROR("Failed to create GLFW window");
		glfwTerminate();
		exit(1);
	}

	glfwSetErrorCallback(error_callback);

	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	glfwSetWindowSizeLimits(window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	int fbW, fbH;
	glfwGetFramebufferSize(window, &fbW, &fbH);

	SB::VkCore::Init();
	vkCore = &SB::VkCore::Get();
	vkCore->AttachCreateSurfaceFunction(CreateWindowSurface);
	vkCore->SetFramebufferSize(fbW, fbH);
	vkCore->InitVk(glfwExtensionCount, glfwExtensions);

	SB::SABLE_LOG("GLFW window created for Vulkan");
}

void App::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

#if defined(__linux__)
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#else
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
#endif

	ImGui_ImplGlfw_InitForVulkan(window, true);
	SB::ImGuiInitInfo SbInitInfo{};
	ImGui_ImplVulkan_InitInfo ImGuiInitInfo{};

	vkCore->GetImGuiInitInfo(SbInitInfo);
	VkRenderPass imguiRenderPass = vkCore->MakeImGuiRenderPass();

	// Copy my cool info over
	ImGuiInitInfo.Instance = SbInitInfo.instance;
	ImGuiInitInfo.PhysicalDevice = SbInitInfo.physicalDevice;
	ImGuiInitInfo.Device = SbInitInfo.device;
	ImGuiInitInfo.Queue = SbInitInfo.graphicsQueue;
	ImGuiInitInfo.DescriptorPool = SbInitInfo.descriptorPool;
	ImGuiInitInfo.ImageCount = SbInitInfo.imageCount;
	ImGuiInitInfo.MinImageCount = SbInitInfo.imageCount;
	ImGuiInitInfo.RenderPass = imguiRenderPass;

	ImGui_ImplVulkan_Init(&ImGuiInitInfo);

	SetupImGuiStyle();

	VkCommandBuffer commandBuffer = vkCore->BeginSingleTimeCommands();
	ImGui_ImplVulkan_CreateFontsTexture();
	vkCore->EndSingleTimeCommands(commandBuffer);

	for (SB::FrameAttachment& image : vkCore->MainFrame.attachments)
	{
		PrepareTextureForImGui(image);
	}
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

void App::ReiszePublicCallback()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);

	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkCore->SetFramebufferSize(width, height);
	vkCore->RecreateSwapchain();
}

void App::Mainloop()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	while (!glfwWindowShouldClose(window))
	{
		frameCount++;
		auto currentTime = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> elapsed = currentTime - lastTime;

		if (elapsed.count() >= 1.0)
		{
			averageFPS = frameCount / elapsed.count();
			lastTime = currentTime;
			frameCount = 0;

			if (currentProject.name.empty())
			{
				glfwSetWindowTitle(window, ("Sable Engine - FPS: " + std::to_string((int)averageFPS)).c_str());
			}
			else
			{
				glfwSetWindowTitle(window, ("Sable Engine - " + currentProject.name + " - FPS: " + std::to_string((int)averageFPS)).c_str());
			}
		}

		glfwPollEvents();

		vkCore->Draw();

		vkCore->BeginFrame();

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		DisplayMenuBar();

		ImGui::DockSpaceOverViewport();

		ImGui::Begin("Docking");
		ImGui::Text("Hello!");
		ImGui::End();

		ImGui::Begin("Viewport");

		static ImVec2 oldSize = ImVec2(0, 0);

		ImVec2 size = ImGui::GetWindowSize();
		size.x -= 10.0f; // Padding
		size.y -= 40.0f; // Padding

		static int countdown = 0;

		if (oldSize.x != size.x || oldSize.y != size.y)
		{
			countdown = vkCore->MainFrame.attachments.size();
			for (SB::FrameAttachment& image : vkCore->MainFrame.attachments)
			{
				ImGui_ImplVulkan_RemoveTexture(image.descriptorSet);
			}

			vkCore->ResizeMainFrame((int)size.x, (int)size.y);
			oldSize = size;

			for (SB::FrameAttachment& image : vkCore->MainFrame.attachments)
			{
				PrepareTextureForImGui(image);
			}
		}

		if (countdown == 0)
		{
			ImGui::Image((ImTextureID)vkCore->GetViewportTexture(), size);
		}
		else
		{
			countdown--;
		}

		ImGui::End();

		if (imguiDemoWindowOpen)
		{
			ImGui::ShowDemoWindow();
		}

		if (consoleWindowOpen)
		{
			SB_EditorConsole::DisplayConsole();
		}

		if (assetManagerWindowOpen)
		{
			fileBrowser->Render();
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

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vkCore->GetCurrentCommandBuffer());

		vkCore->EndFrame();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_window = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_window);
		}
	}
}

App::~App()
{
	vkDeviceWaitIdle(vkCore->GetDevice());

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	vkCore->Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

	delete fileBrowser;

	SBEngine::Shutdown();
}