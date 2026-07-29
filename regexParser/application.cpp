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
	std::ranges::fill(selectionCache, std::vector<int>());
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

		displayMainMenuBar();
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
	setDarkTheme();
	return true;
}

void Application::displayMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Theme"))
		{
			if (ImGui::MenuItem("Light"))
			{
				setLightTheme();
			}
			if (ImGui::MenuItem("Dark"))
			{
				setDarkTheme();
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
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
						ImGui::Indent(20.0f);
						ImGui::TextWrapped("%s", child->getData().c_str());
						ImGui::Indent(20.0f);

						for (const auto& option : child->children)
						{
							if ((option->getTag() == "R" && state != ApplicationState::QUIZ_EXEC) || option->getTag() !=
								"R")
							{
								float wrapWidth = ImGui::GetContentRegionAvail().x;
								ImVec2 textSize =
									ImGui::CalcTextSize(option->getData().c_str(), nullptr, false, wrapWidth);

								ImVec2 cursorPos = ImGui::GetCursorPos();

								if (ImGui::Selectable("##selectableId", false, 0, ImVec2(wrapWidth, textSize.y)))
								{
								}

								ImGui::SetCursorPos(cursorPos);
								ImGui::TextWrapped("%s", option->getData().c_str());
							}
						}
						ImGui::Unindent(40.0f);
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
	static int questionCount{1};

	if (ImGui::Begin("Quiz"))
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
	if (ImGui::Begin("Status"))
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

	ImGui::SetNextItemWidth(300.0f);
	ImGui::SetCursorPosY(windowHeight * 0.3f);
	ImGui::SetCursorPosX((windowWidth - 300.0f) * 0.5f);

	static int values[2]{};
	if (ImGui::InputInt2("##", values))
	{
		values[0] = std::max(0, values[0]);
		values[1] = std::max(0, values[1]);
	}

	ImGui::Spacing();
	ImGui::SetCursorPosX((windowWidth - 200.0f) * 0.5f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.50f, 0.90f, 0.50f, 1.00f));
	if (ImGui::Button("Start", ImVec2(200.0f, 30.0f)))
	{
		if (values[0] && values[1] && values[0] < values[1])
		{
			questionCount = (values[1] - values[0]) + 1;
			fillSet(values[0], values[1]);
		}
		else
		{
			randomizeSet(questionCount);
		}

		state = ApplicationState::QUIZ_EXEC;
	}
	ImGui::PopStyleColor();
}

void Application::displayQuiz(int& questionCount)
{
	static int currentQuestion{};
	int size = document.root->childCount();


	static std::vector<int> selectedResults(maxQuestionOptions, 0);
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
				int resultIdx{};

				ImGui::Indent(20.0f);
				for (const auto& option : document.root->children[i]->children)
				{
					if (option->getTag() != "R")
					{
						float wrapWidth = ImGui::GetContentRegionAvail().x;
						ImVec2 textSize =
							ImGui::CalcTextSize(option->getData().c_str(), nullptr, false, wrapWidth);
						ImVec2 cursorPos = ImGui::GetCursorPos();

						ImGui::PushID(resultIdx);
						ImGui::PushStyleColor(ImGuiCol_Header, ImColors::GreenHeader);

						if (ImGui::Selectable("##", selectedResults[resultIdx] != 0, 0,
						                      ImVec2(wrapWidth, textSize.y)))
						{
							selectedResults[resultIdx] = ~selectedResults[resultIdx];
						}

						ImGui::SetCursorPos(cursorPos);
						ImGui::TextWrapped("%s", option->getData().c_str());

						ImGui::PopID();
						ImGui::PopStyleColor();
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
			bool correct{true};

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
			selectionCache[selector[currentQuestion]] = selectedResults;
			std::ranges::fill(selectedResults, 0);

			currentQuestion = 0;
			state = ApplicationState::QUIZ_FINISH;
		}
	}
	else
	{
		if (ImGui::Button("Next", ImVec2(100.0f, 30.0f)))
		{
			bool correct{true};

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

			selectionCache[selector[currentQuestion]] = selectedResults;
			if (currentQuestion < questionCount - 1)
			{
				currentQuestion += 1;
			}
			std::ranges::fill(selectedResults, 0);
		}
	}
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
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.75f, 0.30f, 0.40f));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.90f, 0.20f, 0.20f, 0.40f));
		}

		if (ImGui::CollapsingHeader(questionData.c_str(), false))
		{
			ImGui::Indent(20.0f);

			int optionIdx{};
			for (const auto& option : document.root->children[current]->children)
			{
				if (optionIdx < maxQuestionOptions && selectionCache[current][optionIdx] == -1)
				{
					if (grade[current] == 1)
					{
						ImGui::PushStyleColor(ImGuiCol_Header, ImColors::GreenHeader);
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_Header, ImColors::RedHeader);
					}
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Header, ImColors::Transparent);
				}

				float wrapWidth = ImGui::GetContentRegionAvail().x;
				ImVec2 textSize =
					ImGui::CalcTextSize(option->getData().c_str(), nullptr, false, wrapWidth);

				ImVec2 cursorPos = ImGui::GetCursorPos();

				if (ImGui::Selectable("##selectableId", true, 0, ImVec2(wrapWidth, textSize.y)))
				{
				}

				ImGui::SetCursorPos(cursorPos);
				ImGui::TextWrapped("%s", option->getData().c_str());

				ImGui::PopStyleColor();
				++optionIdx;
			}
			ImGui::Unindent(20.0f);
		}
		ImGui::PopStyleColor(1);
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

	if (ImGui::Button("Restart"))
	{
		resetQuizData();
		state = ApplicationState::QUIZ_SELECT;
	}

	ImGui::Separator();
}

void Application::randomizeSet(const int& size)
{
	if (!selector.empty())
	{
		selector = {};
	}

	selector = std::vector<int>(1028);
	std::iota(selector.begin(), selector.end(), 0);

	std::random_device rnd{};
	std::mt19937 gen(rnd());
	std::shuffle(selector.begin(), selector.end(), gen);

	selector.resize(size);
}

void Application::fillSet(const int& start, const int& finish)
{
	if (!selector.empty())
	{
		selector = {};
	}

	for (int i = start - 1; i < finish; ++i)
	{
		selector.push_back(i);
	}
}

void Application::resetQuizData()
{
	if (!selector.empty())
	{
		selector = {};
	}
	if (!grade.empty())
	{
		grade = {};
	}
	if (!selectionCache.empty())
	{
		selectionCache = {};
	}
}

std::vector<int> Application::parseResultLine(const std::string& string)
{
	std::vector<int> result(maxQuestionOptions, 0);

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

void Application::setLightTheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	const ImVec4 base = ImVec4(0.93f, 0.94f, 0.96f, 1.00f);
	const ImVec4 mantle = ImVec4(0.90f, 0.91f, 0.94f, 1.00f);
	const ImVec4 surface0 = ImVec4(0.85f, 0.86f, 0.90f, 1.00f);
	const ImVec4 surface1 = ImVec4(0.79f, 0.81f, 0.86f, 1.00f);

	const ImVec4 text = ImVec4(0.30f, 0.31f, 0.38f, 1.00f);
	const ImVec4 subtext0 = ImVec4(0.53f, 0.54f, 0.60f, 1.00f);

	const ImVec4 mauve = ImVec4(0.53f, 0.32f, 0.88f, 1.00f);
	const ImVec4 lavender = ImVec4(0.45f, 0.55f, 0.96f, 1.00f);
	const ImVec4 blue = ImVec4(0.12f, 0.40f, 0.96f, 1.00f);

	colors[ImGuiCol_Text] = text;
	colors[ImGuiCol_TextDisabled] = subtext0;
	colors[ImGuiCol_WindowBg] = base;
	colors[ImGuiCol_ChildBg] = mantle;
	colors[ImGuiCol_PopupBg] = mantle;
	colors[ImGuiCol_Border] = surface1;
	colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

	colors[ImGuiCol_FrameBg] = surface0;
	colors[ImGuiCol_FrameBgHovered] = surface1;
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.72f, 0.75f, 0.81f, 1.00f);

	colors[ImGuiCol_TitleBg] = mantle;
	colors[ImGuiCol_TitleBgActive] = surface0;
	colors[ImGuiCol_TitleBgCollapsed] = mantle;
	colors[ImGuiCol_MenuBarBg] = mantle;

	colors[ImGuiCol_Button] = surface0;
	colors[ImGuiCol_ButtonHovered] = lavender;
	colors[ImGuiCol_ButtonActive] = mauve;

	colors[ImGuiCol_Header] = ImVec4(0.53f, 0.32f, 0.88f, 0.20f);
	colors[ImGuiCol_HeaderHovered] = lavender;
	colors[ImGuiCol_HeaderActive] = mauve;

	colors[ImGuiCol_Tab] = mantle;
	colors[ImGuiCol_TabHovered] = lavender;
	colors[ImGuiCol_TabActive] = base;
	colors[ImGuiCol_TabUnfocused] = mantle;
	colors[ImGuiCol_TabUnfocusedActive] = base;

	colors[ImGuiCol_CheckMark] = mauve;
	colors[ImGuiCol_SliderGrab] = mauve;
	colors[ImGuiCol_SliderGrabActive] = lavender;
	colors[ImGuiCol_ScrollbarBg] = base;
	colors[ImGuiCol_ScrollbarGrab] = surface0;
	colors[ImGuiCol_ScrollbarGrabHovered] = surface1;
	colors[ImGuiCol_ScrollbarGrabActive] = mauve;
	colors[ImGuiCol_NavHighlight] = mauve;

	style.WindowRounding = 8.0f;
	style.FrameRounding = 5.0f;
	style.PopupRounding = 6.0f;
	style.ScrollbarRounding = 8.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 5.0f;
	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.WindowPadding = ImVec2(12, 12);
	style.FramePadding = ImVec2(8, 5);
	style.ItemSpacing = ImVec2(8, 6);
}

void Application::setDarkTheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	const ImVec4 crust = ImVec4(0.07f, 0.07f, 0.10f, 1.00f);
	const ImVec4 base = ImVec4(0.10f, 0.11f, 0.15f, 1.00f);
	const ImVec4 mantle = ImVec4(0.13f, 0.14f, 0.19f, 1.00f);
	const ImVec4 surface0 = ImVec4(0.18f, 0.19f, 0.26f, 1.00f);
	const ImVec4 surface1 = ImVec4(0.24f, 0.25f, 0.34f, 1.00f);
	const ImVec4 surface2 = ImVec4(0.31f, 0.32f, 0.43f, 1.00f);

	const ImVec4 text = ImVec4(0.89f, 0.91f, 0.95f, 1.00f);
	const ImVec4 subtext0 = ImVec4(0.61f, 0.63f, 0.71f, 1.00f);

	const ImVec4 accent = ImVec4(0.53f, 0.38f, 0.96f, 1.00f);
	const ImVec4 accent_hover = ImVec4(0.63f, 0.49f, 0.98f, 1.00f);
	const ImVec4 accent_active = ImVec4(0.44f, 0.28f, 0.88f, 1.00f);

	colors[ImGuiCol_Text] = text;
	colors[ImGuiCol_TextDisabled] = subtext0;
	colors[ImGuiCol_WindowBg] = base;
	colors[ImGuiCol_ChildBg] = mantle;
	colors[ImGuiCol_PopupBg] = crust;
	colors[ImGuiCol_Border] = surface1;
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

	colors[ImGuiCol_FrameBg] = surface0;
	colors[ImGuiCol_FrameBgHovered] = surface1;
	colors[ImGuiCol_FrameBgActive] = surface2;

	colors[ImGuiCol_TitleBg] = crust;
	colors[ImGuiCol_TitleBgActive] = mantle;
	colors[ImGuiCol_TitleBgCollapsed] = crust;
	colors[ImGuiCol_MenuBarBg] = mantle;

	colors[ImGuiCol_Button] = surface0;
	colors[ImGuiCol_ButtonHovered] = accent_hover;
	colors[ImGuiCol_ButtonActive] = accent_active;

	colors[ImGuiCol_Header] = ImVec4(0.53f, 0.38f, 0.96f, 0.25f);
	colors[ImGuiCol_HeaderHovered] = accent_hover;
	colors[ImGuiCol_HeaderActive] = accent_active;

	colors[ImGuiCol_Tab] = mantle;
	colors[ImGuiCol_TabHovered] = accent_hover;
	colors[ImGuiCol_TabActive] = base;
	colors[ImGuiCol_TabUnfocused] = mantle;
	colors[ImGuiCol_TabUnfocusedActive] = base;

	colors[ImGuiCol_CheckMark] = accent;
	colors[ImGuiCol_SliderGrab] = accent;
	colors[ImGuiCol_SliderGrabActive] = accent_hover;
	colors[ImGuiCol_ScrollbarBg] = base;
	colors[ImGuiCol_ScrollbarGrab] = surface0;
	colors[ImGuiCol_ScrollbarGrabHovered] = surface1;
	colors[ImGuiCol_ScrollbarGrabActive] = accent;
	colors[ImGuiCol_NavHighlight] = accent;

	style.WindowRounding = 8.0f;
	style.FrameRounding = 5.0f;
	style.PopupRounding = 6.0f;
	style.ScrollbarRounding = 8.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 5.0f;
	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.WindowPadding = ImVec2(12, 12);
	style.FramePadding = ImVec2(8, 5);
	style.ItemSpacing = ImVec2(8, 6);
}
