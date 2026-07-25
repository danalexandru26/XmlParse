#include<iostream>
#include<fstream>

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

	RendererStatus status = renderer.ininitializeRenderer();
	if (status == RendererStatus::RENDERER_VALID)
	{
		while (!renderer.shouldClose())
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glfwPollEvents();
			renderer.swapBuffers();
		}
	}

	return 0;
}

void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
