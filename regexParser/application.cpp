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

	std::ranges::fill(grade, 0);
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

	if (!document.setRoot(std::move(root)))
	{
		std::cerr << "XML document root could not be configured\n";
		return false;
	}

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
							if ((option->getTag() == "R" && state != ApplicationState::QUIZ_EXEC) || option->getTag() !=
								"R")
							{
								ImGui::Selectable(option->getData().c_str());
							}
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
		case ApplicationState::QUIZ_FINISH:
			{
				displayQuizResults(questionCount);
			}
			break;
		}
	}
	ImGui::End();
}

void Application::displayStatusWindow()
{
	if (ImGui::Begin("Status Window"))
	{
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
		randomizeSet(questionCount);
		state = ApplicationState::QUIZ_EXEC;
	}
	ImGui::PopStyleColor();
}

void Application::displayQuiz(int& questionCount)
{
	static int currentQuestion{};
	int size = document.root->childCount();


	static std::vector<int> selectedResults(6, 0);
	std::vector<int> result{};
	std::string statusLabel = std::format("Question {}/{}", currentQuestion + 1, questionCount);

	ImGui::TextEx(statusLabel.c_str());
	ImGui::Spacing();


	for (int i = 0; i < size; ++i)
	{
		if (selector[currentQuestion] == i)
		{
			const std::string questionId = document.root->children[i]->getTag();
			const auto questionText = document.root->children[i]->getData();

			if (ImGui::CollapsingHeader(questionText.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				size_t resultIdx{};

				ImGui::Indent(20.0f);
				for (const auto& option : document.root->children[i]->children)
				{
					if (option->getTag() != "R")
					{
						if (ImGui::Selectable(option->getData().c_str(), selectedResults[resultIdx] != 0))
						{
							selectedResults[resultIdx] = ~selectedResults[resultIdx];
						}
					}
					else if (option->getTag() == "R")
					{
						result = parseResultLine(option->getData());
					}
					++resultIdx;
				}
				ImGui::Unindent(20.0f);
			}
		}
	}
	ImGui::Spacing();

	float windowWidth = ImGui::GetContentRegionAvail().x;
	ImGui::SetCursorPosX((windowWidth - 200.0f) * 0.5f);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.82f, 0.94f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.78f, 0.74f, 0.90f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.68f, 0.62f, 0.85f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.20f, 0.20f, 0.30f, 1.00f));
	if (ImGui::Button("Previous", ImVec2(100.0f, 30.0f)))
	{
		if (currentQuestion > 0)
		{
			currentQuestion -= 1;
		}
		std::ranges::fill(selectedResults, 0);
	}

	ImGui::SameLine();
	if (currentQuestion + 1 == questionCount)
	{
		if (ImGui::Button("Finish", ImVec2(100.0f, 30.0f)))
		{
			state = ApplicationState::QUIZ_FINISH;
			bool correct{true};

			for (auto& n : result)
			{
				std::cout << n << ' ';
			}
			std::cout << '\n';

			for (auto& n : selectedResults)
			{
				std::cout << n << ' ';
			}
			std::cout << '\n';

			for (size_t i = 0; i < result.size() && correct; ++i)
			{
				if (result[i] != selectedResults[i])
				{
					grade[selector[currentQuestion]] = 0;
					correct = false;
				}
			}
			if (correct == true)
			{
				grade[selector[currentQuestion]] = 1;
			}

			std::ranges::fill(selectedResults, 0);
		}
	}
	else
	{
		if (ImGui::Button("Next", ImVec2(100.0f, 30.0f)))
		{
			bool correct{true};

			for (auto& n : result)
			{
				std::cout << n << ' ';
			}
			std::cout << '\n';

			for (auto& n : selectedResults)
			{
				std::cout << n << ' ';
			}
			std::cout << '\n';

			for (size_t i = 0; i < result.size() && correct; ++i)
			{
				if (result[i] != selectedResults[i])
				{
					grade[selector[currentQuestion]] = 0;
					correct = false;
				}
			}
			if (correct == true)
			{
				std::cerr << selector[currentQuestion];
				grade[selector[currentQuestion]] = 1;
			}

			if (currentQuestion < questionCount - 1)
			{
				currentQuestion += 1;
			}

			std::ranges::fill(selectedResults, 0);
		}
	}
	ImGui::PopStyleColor(4);
}

void Application::displayQuizResults(int& questionCount)
{
	int size = document.root->childCount();

	for (int i = 0; i < selector.size(); ++i)
	{
		int current = selector[i];

		std::string questionTag = document.root->children[current]->getTag();
		std::string questionData = document.root->children[current]->getData();

		if (grade[current] == 1)
		{
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.78f, 0.90f, 0.80f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.70f, 0.85f, 0.73f, 1.0f));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.95f, 0.78f, 0.78f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.90f, 0.68f, 0.68f, 1.0f));
		}
		if (ImGui::CollapsingHeader(questionData.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Indent(20.0f);
			for (const auto& option : document.root->children[current]->children)
			{
				ImGui::Selectable(option->getData().c_str());
			}
			ImGui::Unindent();
		}
		ImGui::PopStyleColor(2);
	}

	int correct{};
	for (const auto& n : grade)
	{
		if (n == 1)
		{
			correct += 1;
		}
	}

	std::string summary = std::format("{}/{}. {} Correct out of {} Total", correct, questionCount, correct,
	                                  questionCount);

	ImGui::Spacing();
	ImGui::TextEx(summary.c_str());
	ImGui::Separator();
}

void Application::randomizeSet(const int& size)
{
	if (!selector.empty())
	{
		selector.clear();
	}
	selector = std::vector<int>(1028);
	std::iota(selector.begin(), selector.end(), 0);

	std::random_device rnd{};
	std::mt19937 gen(rnd());
	std::shuffle(selector.begin(), selector.end(), gen);

	selector.resize(size);
}

std::vector<int> Application::parseResultLine(const std::string& string)
{
	std::vector<int> result(6, 0);

	for (const auto& c : string)
	{
		if (c == '(')
		{
			break;
		}
		if (c != 'R' && isalpha(c))
		{
			result[tolower(c) - 'a'] = ~result[tolower(c) - 'a'];
		}
	}

	return result;
}
