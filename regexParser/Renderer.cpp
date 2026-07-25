#include"Renderer.h"

#include <imgui.h>

Renderer::Renderer(const std::pair<int, int>& version, int width, int height, std::string title)
	: width{width}, height{height}, title{title}
{
	if (validateGLVersion(version) == RendererStatus::OPENGL_VERSION_INVALID)
	{
		std::cerr << "OpenGL version is not valid. Renderer terminated\n";
		state = RendererStatus::RENDERER_INVALID;
	}
	else
	{
		glVersion = version;
		state = RendererStatus::RENDERER_VALID;
	}
}

Renderer::~Renderer()
{
	terminateGLFW();
}

RendererStatus Renderer::ininitializeRenderer()
{
	RendererStatus status{};

	status = initializeGLFW();
	if (status != RendererStatus::GLFW_INITIALIZE_SUCCESS)
	{
		return RendererStatus::RENDERER_INVALID;
	}

	status = constructWindow();
	if (status != RendererStatus::WINDOW_HANDLER_SUCCESS)
	{
		return RendererStatus::RENDERER_INVALID;
	}

	status = initializeGLAD();
	if (status != RendererStatus::GLAD_INITIALIZE_SUCCESS)
	{
		return RendererStatus::RENDERER_INVALID;
	}

	glViewport(0, 0, width, height);

	return RendererStatus::RENDERER_VALID;
}

RendererStatus Renderer::initializeGLFW()
{
	if (state == RendererStatus::RENDERER_INVALID) return state;

	int status = glfwInit();
	if (status == GLFW_FALSE)
	{
		std::cerr << "GLFW could not be initialized\n";

		state = RendererStatus::RENDERER_INVALID;
		return RendererStatus::GLFW_INITIALIZE_ERROR;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersion.first);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersion.second);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return RendererStatus::GLFW_INITIALIZE_SUCCESS;
}

RendererStatus Renderer::initializeGLAD()
{
	if (state == RendererStatus::RENDERER_INVALID) return state;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "GLAD could not be initialized\n";

		state = RendererStatus::RENDERER_INVALID;
		return RendererStatus::GLAD_INITIALIZE_FAILURE;
	}

	return RendererStatus::GLAD_INITIALIZE_SUCCESS;
}

RendererStatus Renderer::constructWindow()
{
	if (state == RendererStatus::RENDERER_INVALID) return state;

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window != nullptr)
	{
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

		return RendererStatus::WINDOW_HANDLER_SUCCESS;
	}

	state = RendererStatus::RENDERER_INVALID;
	return RendererStatus::WINDOW_HANDLER_FAILURE;
}

bool Renderer::shouldClose()
{
	if (window == nullptr || state == RendererStatus::RENDERER_INVALID) return true;

	return glfwWindowShouldClose(window);
}

void Renderer::terminateGLFW()
{
	glfwTerminate();
}

RendererStatus Renderer::validateGLVersion(const std::pair<int, int>& version) const
{
	if (version.first <= 2 ||
		(version.first == 3 && version.second <= 2) ||
		(version.first == 4 || version.second >= 7) ||
		version.first > 4)
	{
		return RendererStatus::OPENGL_VERSION_INVALID;
	}
	return RendererStatus::OPENGL_VERSION_VALID;
}
