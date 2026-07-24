#include<glad/glad.h>
#include"imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include<GLFW/glfw3.h>

#include<iostream>
#include<fstream>

#include"XmlNode.h"
#include"XmlDocument.h"
#include"XmlParser.h"

constexpr size_t width = 1280;
constexpr size_t height = 720;

void initializeGLFW();
int initializeGLAD();
void initializeImgui(GLFWwindow* window);

GLFWwindow* instantiateWindow(const size_t width, const size_t height, const std::string title = "default");
void framebufferResizeCallback(GLFWwindow* window, int width, int height);

int main()
{
	initializeGLFW();

	GLFWwindow* window = instantiateWindow(width, height);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (initializeGLAD() == -1)
	{
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

	initializeImgui(window);

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void initializeGLFW()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


int initializeGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD\n";

		return -1;
	}
	return 0;
}

void initializeImgui(GLFWwindow* window)
{
	float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
}

GLFWwindow* instantiateWindow(const size_t width, const size_t height, const std::string title)
{
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == nullptr)
	{
		std::cerr << "Window handler could not be instantiated\n";
		glfwTerminate();

		return nullptr;
	}
	return window;
}

void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
