#include"application.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

Application::Application(std::pair<int, int> version, const int& width, const int& height,
                         const std::string& title)
	: title{title}
{
	renderer = Renderer(version, width, height, title);
}

bool Application::readText(const std::string& path)
{
	return true;
}

bool Application::readXml(const std::string& path)
{
	return true;
}

bool Application::start()
{
	if (!setupRenderer())
	{
		std::cerr << "Application cannot start. Consult the error message stack above\n";
		return false;
	}

	std::cerr << "Starting Application\n";
	while (!renderer.shouldClose())
	{
		renderer.pollEvents();

		displayDefaultScreen();

		renderer.clearColor(defaultColor, GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		renderer.swapBuffers();
	}

	return true;
}

bool Application::setupRenderer()
{
	if (!renderer.initializeRenderer())
	{
		std::cerr << "Error during Renderer initialization\n";
		return false;
	}

	if (!renderer.initializeImGui())
	{
		std::cerr << "Error during ImGui initialization\n";
		return false;
	}
	renderer.applyGruvboxImGui();

	return true;
}

void Application::displayDefaultScreen()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();


	ImGui::Render();
}
