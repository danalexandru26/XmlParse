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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();

		displayDropDown();

		ImGui::Render();
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
	ImGui::ShowDemoWindow();
}

void Application::displayDropDown()
{
	static int questionId{};
	static const char* loremParagraph = R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit. 
Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, 
quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute 
irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.)";


	if (ImGui::Begin("Questions"))
	{
		ImGui::InputInt("questionId", &questionId);
		ImGui::Separator();
		ImGui::Indent(10.0f);

		if (ImGui::BeginChild("Scrollable", ImVec2(0, 0), false))
		{
			for (int i = 0; i < 100; ++i)
			{
				std::string questionHeader = "Question " + std::to_string(i);
				if (ImGui::CollapsingHeader(questionHeader.c_str()))
				{
				ImGui:;
					ImGui::TextWrapped("%s", loremParagraph);
				}
				ImGui::Separator();
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}
