#include<iostream>
#include<glad/glad.h>
#include <imgui.h>

#include"XmlNode.h"
#include"XmlDocument.h"
#include"XmlParser.h"
#include"Renderer.h"

constexpr size_t width = 1280;
constexpr size_t height = 720;

void framebufferResizeCallback(GLFWwindow* window, int width, int height);

int WinMain()
{
	Renderer renderer({3, 3}, width, height, "Application");

	bool status = renderer.initializeRenderer();
	if (status == false)
	{
		std::cerr << "Renderer could not be initialized\n";
		return 0;
	}

	while (!renderer.shouldClose())
	{
		renderer.pollEvents();

		renderer.clearColor({0.1f, 0.1f, 0.1f, 1.0f}, GL_COLOR_BUFFER_BIT);
		renderer.swapBuffers();
	}

	return 0;
}

void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
