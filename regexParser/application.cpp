#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include"application.h"

Application::Application(std::pair<int, int> version, const int& width, const int& height,
                         const std::string& title)
	: title{title}
{
	state = ApplicationState::QUIZ_SELECT;
	renderer = Renderer(version, width, height, title);
}

bool Application::readText(const std::string& path)
{
	parser.configurePath(path);

	std::unique_ptr<XmlNode> root = parser.txtToXml();

	if (root == nullptr)
	{
		std::cerr << "Text document could not be parsed to XML\n";
		return false;
	}

	if (document.setRoot(std::move(root)))
	{
		std::cerr << "XML document root successfully configured\n";
		return true;
	}
	return false;
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

	if (!readText(defaultPathText))
	{
		std::cerr << "Error parsing text to XML\n";
		return false;
	}

	std::cerr << "Starting Application\n";
	while (!renderer.shouldClose())
	{
		renderer.pollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiDockNodeFlags flags = ImGuiDockNodeFlags_PassthruCentralNode;
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), flags);

		displayDropDown();
		displayMainWindow();
		displayStatusWindow();

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
	renderer.styleLight();

	return true;
}

void Application::displayDropDown()
{
	static int questionId{};

	if (ImGui::Begin("Questions"))
	{
		ImGui::Indent(10.0f);
		ImGui::PushItemWidth(200.0f);
		ImGui::InputInt("Question ID", &questionId);
		ImGui::Separator();

		if (document.root != nullptr)
		{
			int currentLoopID{};

			if (ImGui::BeginChild("Question", ImVec2(0, 0), false))
			{
				for (const auto& child : document.root->children)
				{
					if (ImGui::CollapsingHeader(child->getTag().c_str()))
					{
						ImGui::TextWrapped("%s", child->getData().c_str());

						ImGui::Indent(20.0f);
						for (const auto& option : child->children)
						{
							ImGui::PushID(currentLoopID);
							ImGui::Selectable(option->getData().c_str());
							ImGui::PopID();
						}
						ImGui::Unindent(20.0f);
					}
					ImGui::Separator();
				}
			}
			ImGui::EndChild();
		}
	}
	ImGui::End();
}

void Application::displayMainWindow()
{
	static int questionCount{};
	questionCount = std::max(0, questionCount);

	if (ImGui::Begin("Main Window"))
	{
		switch (state)
		{
		case ApplicationState::QUIZ_SELECT:
			{
				displayQuizSelection(questionCount);
			}
			break;
		case ApplicationState::QUIZ_EXEC:
			{
				displayQuiz(questionCount);
			}
			break;
		}
	}
	ImGui::End();
}

void Application::displayQuizSelection(int& questionCount)
{
	float textWidth = ImGui::CalcTextSize("Quiz Configuration").x;
	float windowWidth = ImGui::GetContentRegionAvail().x;
	float windowHeight = ImGui::GetContentRegionAvail().y;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::TextEx("Quiz Configuration");

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	textWidth = ImGui::CalcTextSize("Question Count").x;
	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::TextEx("Question Count");
	ImGui::Spacing();

	ImGui::SetNextItemWidth(400.0f);
	ImGui::SetCursorPosY(windowHeight * 0.2f);
	ImGui::SetCursorPosX((windowWidth - 400.0f) * 0.5f);
	ImGui::InputInt("##Number of Questions", &questionCount);
	ImGui::Spacing();

	ImGui::SetCursorPosX((windowWidth - 200.0f) * 0.5f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.50f, 0.90f, 0.50f, 1.00f));
	if (ImGui::Button("Start", ImVec2(200.0f, 30.0f)))
	{
		state = ApplicationState::QUIZ_EXEC;
	}
	ImGui::PopStyleColor();
}

void Application::displayQuiz(int& questionCount)
{
	if (ImGui::Button("switch"))
	{
		state = ApplicationState::QUIZ_SELECT;
	}
}


void Application::displayStatusWindow()
{
	if (ImGui::Begin("Status Window"))
	{
	}
	ImGui::End();
}
