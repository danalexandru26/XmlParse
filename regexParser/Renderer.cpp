#include"Renderer.h"

Renderer::Renderer(const std::pair<int, int>& version, size_t width, size_t height, std::string title)
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

RendererStatus Renderer::initializeGLFW()
{
	if (state == RendererStatus::RENDERER_INVALID) return state;

	int status = glfwInit();
	if (status == GLFW_FALSE)
	{
		std::cerr << "GLFW could not be initialized\n";
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

	return RendererStatus::GLAD_INITIALIZE_SUCCESS;
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
