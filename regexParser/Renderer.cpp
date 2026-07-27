#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include"Renderer.h"

Renderer::Renderer(const std::pair<int, int>& version, int width, int height, std::string title)
	: version{version}, width{width}, height{height}, title{title}
{
}

Renderer::~Renderer()
{
	window = nullptr;
	glfwTerminate();
}

bool Renderer::initializeRenderer()
{
	if (initializeGLFW() == false) { return false; }
	if (constructWindow() == false) { return false; }
	if (initializeGLAD() == false) { return false; }

	glViewport(0, 0, width, height);

	return true;
}

bool Renderer::initializeGLFW()
{
	int status = glfwInit();
	if (status == GLFW_FALSE)
	{
		std::cerr << "GLFW could not be initialized\n";

		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.first);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.second);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return true;
}

bool Renderer::initializeGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "GLAD could not be initialized\n";

		return false;
	}
	return true;
}

bool Renderer::initializeImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
	{
		std::cerr << "ImGui GLFW Implementation cannot be initialized\n";
		return false;
	}
	if (!ImGui_ImplOpenGL3_Init(nullptr))
	{
		std::cerr << "ImGui OpenGL3 Implementation cannot be initialized\n";
		return false;
	}
	return true;
}

bool Renderer::constructWindow()
{
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Window could not be initialized\n";
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	return true;
}

bool Renderer::shouldClose()
{
	if (window == nullptr) return true;

	return glfwWindowShouldClose(window);
}

void Renderer::setResizeCallback(GLFWframebuffersizefun func)
{
	if (window != nullptr)
	{
		glfwSetFramebufferSizeCallback(window, func);
	}
}

void Renderer::swapBuffers()
{
	glfwSwapBuffers(window);
}

void Renderer::pollEvents()
{
	glfwPollEvents();
}

void Renderer::clearColor(std::array<float, 4> color, GLbitfield mask)
{
	glClearColor(color[0], color[1], color[2], color[3]);
	glClear(mask);
}

void Renderer::applyGruvboxImGui()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// --- Geometry & Rounding ---
	style.WindowRounding = 4.0f;
	style.ChildRounding = 4.0f;
	style.FrameRounding = 3.0f;
	style.PopupRounding = 4.0f;
	style.ScrollbarRounding = 4.0f;
	style.GrabRounding = 3.0f;
	style.TabRounding = 4.0f;

	// --- Borders & Padding ---
	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 6.0f);
	style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);

	// --- Gruvbox Palette Definitions ---
	const ImVec4 bg0_hard = ImVec4(0.114f, 0.122f, 0.129f, 1.00f); // #1d2021
	const ImVec4 bg0 = ImVec4(0.157f, 0.157f, 0.157f, 1.00f); // #282828
	const ImVec4 bg1 = ImVec4(0.235f, 0.220f, 0.212f, 1.00f); // #3c3836
	const ImVec4 bg2 = ImVec4(0.314f, 0.286f, 0.271f, 1.00f); // #504945
	const ImVec4 bg3 = ImVec4(0.400f, 0.361f, 0.333f, 1.00f); // #665c54

	const ImVec4 fg0 = ImVec4(0.984f, 0.961f, 0.894f, 1.00f); // #fbf1c7
	const ImVec4 fg1 = ImVec4(0.922f, 0.859f, 0.698f, 1.00f); // #ebdbb2
	const ImVec4 fg4 = ImVec4(0.659f, 0.600f, 0.518f, 1.00f); // #a89984

	const ImVec4 yellow = ImVec4(0.980f, 0.741f, 0.184f, 1.00f); // #fabd2f
	const ImVec4 orange = ImVec4(0.980f, 0.490f, 0.196f, 1.00f); // #fe8019
	const ImVec4 aqua = ImVec4(0.537f, 0.753f, 0.525f, 1.00f); // #8ec07c
	const ImVec4 green = ImVec4(0.722f, 0.733f, 0.149f, 1.00f); // #b8bb26

	// --- Backgrounds & Popups ---
	colors[ImGuiCol_WindowBg] = bg0;
	colors[ImGuiCol_ChildBg] = bg0_hard;
	colors[ImGuiCol_PopupBg] = bg1;

	// --- Borders ---
	colors[ImGuiCol_Border] = bg2;
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

	// --- Text ---
	colors[ImGuiCol_Text] = fg1;
	colors[ImGuiCol_TextDisabled] = fg4;

	// --- Menu Bar & Header Buttons ---
	colors[ImGuiCol_MenuBarBg] = bg0_hard; // Explicit contrast behind menu bars
	colors[ImGuiCol_Header] = bg1; // Idle state for menu options / selectable headers
	colors[ImGuiCol_HeaderHovered] = bg2;
	colors[ImGuiCol_HeaderActive] = bg3;

	// --- Standard Buttons ---
	colors[ImGuiCol_Button] = bg2;
	colors[ImGuiCol_ButtonHovered] = ImVec4(orange.x, orange.y, orange.z, 0.70f);
	colors[ImGuiCol_ButtonActive] = orange;

	// --- Frame Backgrounds & Controls ---
	colors[ImGuiCol_FrameBg] = bg1;
	colors[ImGuiCol_FrameBgHovered] = bg2;
	colors[ImGuiCol_FrameBgActive] = bg3;
	colors[ImGuiCol_CheckMark] = yellow;
	colors[ImGuiCol_SliderGrab] = yellow;
	colors[ImGuiCol_SliderGrabActive] = orange;

	// --- Tabs & Docking ---
	colors[ImGuiCol_Tab] = bg0_hard;
	colors[ImGuiCol_TabHovered] = bg2;
	colors[ImGuiCol_TabActive] = bg1;
	colors[ImGuiCol_TabUnfocused] = bg0_hard;
	colors[ImGuiCol_TabUnfocusedActive] = bg1;
	colors[ImGuiCol_DockingPreview] = ImVec4(yellow.x, yellow.y, yellow.z, 0.40f);
	colors[ImGuiCol_DockingEmptyBg] = bg0_hard;

	// --- Title Bars ---
	colors[ImGuiCol_TitleBg] = bg0_hard;
	colors[ImGuiCol_TitleBgActive] = bg1;
	colors[ImGuiCol_TitleBgCollapsed] = bg0_hard;

	// --- Scrollbars ---
	colors[ImGuiCol_ScrollbarBg] = bg0_hard;
	colors[ImGuiCol_ScrollbarGrab] = bg2;
	colors[ImGuiCol_ScrollbarGrabHovered] = bg3;
	colors[ImGuiCol_ScrollbarGrabActive] = fg4;

	// --- Separators & Grips ---
	colors[ImGuiCol_Separator] = bg2;
	colors[ImGuiCol_SeparatorHovered] = yellow;
	colors[ImGuiCol_SeparatorActive] = orange;
	colors[ImGuiCol_ResizeGrip] = bg2;
	colors[ImGuiCol_ResizeGripHovered] = yellow;
	colors[ImGuiCol_ResizeGripActive] = orange;

	// --- Focus Highlights ---
	colors[ImGuiCol_NavCursor] = ImVec4(0.980f, 0.490f, 0.196f, 1.00f); // Orange focus cursor
	colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.980f, 0.741f, 0.184f, 1.00f); // Yellow tab overline
}

bool Renderer::validateGLVersion(const std::pair<int, int>& version) const
{
	if (version.first <= 2 ||
		(version.first == 3 && version.second <= 2) ||
		(version.first == 4 || version.second >= 7) ||
		version.first > 4)
	{
		return false;
	}
	return true;
}
