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

	setupFont(io, ro_glyph_ranges);

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

void Renderer::setupFont(ImGuiIO& io, const ImWchar gylphRange[])
{
	ImFont* font = io.Fonts->AddFontFromFileTTF(
		"C:\\Windows\\Fonts\\segoeui.ttf",
		18.0f,
		nullptr,
		ro_glyph_ranges
	);
}

void Renderer::styleLight()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Catppuccin Latte Palette
	const ImVec4 base = ImVec4(0.93f, 0.94f, 0.96f, 1.00f); // #EFF1F5
	const ImVec4 mantle = ImVec4(0.90f, 0.91f, 0.94f, 1.00f); // #E6E9EF
	const ImVec4 surface0 = ImVec4(0.85f, 0.86f, 0.90f, 1.00f); // #CCD0DA
	const ImVec4 surface1 = ImVec4(0.79f, 0.81f, 0.86f, 1.00f); // #BCC0CC

	const ImVec4 text = ImVec4(0.30f, 0.31f, 0.38f, 1.00f); // #4C4F69
	const ImVec4 subtext0 = ImVec4(0.53f, 0.54f, 0.60f, 1.00f); // #8C8FA1

	const ImVec4 mauve = ImVec4(0.53f, 0.32f, 0.88f, 1.00f); // Primary Accent (#8839EF)
	const ImVec4 lavender = ImVec4(0.45f, 0.55f, 0.96f, 1.00f); // Hover Accent (#7287FD)
	const ImVec4 blue = ImVec4(0.12f, 0.40f, 0.96f, 1.00f); // #1E66F5

	colors[ImGuiCol_Text] = text;
	colors[ImGuiCol_TextDisabled] = subtext0;
	colors[ImGuiCol_WindowBg] = base;
	colors[ImGuiCol_ChildBg] = mantle;
	colors[ImGuiCol_PopupBg] = mantle;
	colors[ImGuiCol_Border] = surface1;
	colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

	colors[ImGuiCol_FrameBg] = surface0;
	colors[ImGuiCol_FrameBgHovered] = surface1;
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.72f, 0.75f, 0.81f, 1.00f);

	colors[ImGuiCol_TitleBg] = mantle;
	colors[ImGuiCol_TitleBgActive] = surface0;
	colors[ImGuiCol_TitleBgCollapsed] = mantle;
	colors[ImGuiCol_MenuBarBg] = mantle;

	colors[ImGuiCol_Button] = surface0;
	colors[ImGuiCol_ButtonHovered] = lavender;
	colors[ImGuiCol_ButtonActive] = mauve;

	colors[ImGuiCol_Header] = ImVec4(0.53f, 0.32f, 0.88f, 0.20f);
	colors[ImGuiCol_HeaderHovered] = lavender;
	colors[ImGuiCol_HeaderActive] = mauve;

	colors[ImGuiCol_Tab] = mantle;
	colors[ImGuiCol_TabHovered] = lavender;
	colors[ImGuiCol_TabActive] = base;
	colors[ImGuiCol_TabUnfocused] = mantle;
	colors[ImGuiCol_TabUnfocusedActive] = base;

	colors[ImGuiCol_CheckMark] = mauve;
	colors[ImGuiCol_SliderGrab] = mauve;
	colors[ImGuiCol_SliderGrabActive] = lavender;
	colors[ImGuiCol_ScrollbarBg] = base;
	colors[ImGuiCol_ScrollbarGrab] = surface0;
	colors[ImGuiCol_ScrollbarGrabHovered] = surface1;
	colors[ImGuiCol_ScrollbarGrabActive] = mauve;
	colors[ImGuiCol_NavHighlight] = mauve;

	style.WindowRounding = 8.0f;
	style.FrameRounding = 5.0f;
	style.PopupRounding = 6.0f;
	style.ScrollbarRounding = 8.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 5.0f;
	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.WindowPadding = ImVec2(12, 12);
	style.FramePadding = ImVec2(8, 5);
	style.ItemSpacing = ImVec2(8, 6);
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
