#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <imgui.h>

#include"Renderer.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

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

	ImGui::StyleColorsLight();

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

void Renderer::styleImGui()
{
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
