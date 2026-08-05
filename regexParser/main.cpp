#include<iostream>
#include<glad/glad.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "application.h"
#include"XmlNode.h"
#include"XmlDocument.h"
#include"XmlParser.h"
#include"Renderer.h"

constexpr size_t width = 1280;
constexpr size_t height = 720;

int WinMain()
{
	Application app({3, 3}, width, height, "QuizGen");

	if (!app.start())
	{
		std::cerr << "Application failed to start\n";
		return 1;
	}

	return 0;
}
