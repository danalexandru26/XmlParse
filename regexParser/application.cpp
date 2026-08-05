#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include"application.h"

Application::Application(std::pair<int, int> version, const int& width, const int& height,
                         const std::string& title)
	: title{title}
{
	bold22 = nullptr;
	bold18 = nullptr;

	currentQuestion = 0;
	questionCount = ApplicationDefault::minimumQuestionCount;

	state = ApplicationState::QUIZ_SELECT;
	renderer = Renderer(version, width, height, title);

	std::ranges::fill(grade, 0);
	std::ranges::fill(selectionCache, std::vector<int>());
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

	bold22 = configureFont(SystemPath::defaultFont, 22.0f);
	bold18 = configureFont(SystemPath::defaultFont, 18.0f);

	setDarkTheme();

	return true;
}

bool Application::readText(const std::string_view& path)
{
	parser.configurePath(std::string(path));

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

ImFont* Application::configureFont(const std::string_view& path, float size)
{
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF(std::string(path).c_str(), size);

	return font;
}

bool Application::start()
{
	if (!setupRenderer())
	{
		std::cerr << "Application cannot start. Consult the error message stack above\n";
		return false;
	}

	if (!readText(SystemPath::defaultPathText))
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
		renderer.clearColor(ApplicationDefault::defaultColor, GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		renderer.swapBuffers();
	}
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
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Restart"))
			{
				resetQuizData();
				state = ApplicationState::QUIZ_SELECT;
			}

			if (state == ApplicationState::QUIZ_EXEC)
			{
				if (ImGui::MenuItem("Finish"))
				{
					state = ApplicationState::QUIZ_FINISH;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Application::displayDropDown()
{
	static int questionId{};
	static std::array<int, 1028> selected{};

	if (ImGui::Begin("Questions"))
	{
		ImGui::SetNextItemWidth(200.0f);
		ImGui::InputInt("Question", &questionId);
		ImGui::Separator();

		if (document.root != nullptr)
		{
			if (ImGui::BeginChild("Question", ImVec2(0, 0), true))
			{
				int current{};
				for (const auto& child : document.root->children)
				{
					ImGui::PushID(current);
					if (displaySelectableWrap(child->getData(), &selected[current]))
					{
						for (const auto& option : child->children)
						{
							if ((option->getTag() == "R" && state != ApplicationState::QUIZ_EXEC) || option->getTag() !=
								"R")
							{
								ImGui::PushStyleColor(ImGuiCol_Header, ImColors::OrangeHeader);
								displaySelectableWrap(option->getData(), nullptr);
								ImGui::PopStyleColor();
							}
						}
					}

					ImGui::Separator();
					ImGui::PopID();

					current += 1;
				}
			}
			ImGui::EndChild();
		}
	}
	ImGui::End();
}

void Application::displayMainWindow()
{
	if (ImGui::Begin("Quiz"))
	{
		switch (state)
		{
		case ApplicationState::QUIZ_SELECT:
			{
				displayQuizSelection();
			}
			break;
		case ApplicationState::QUIZ_EXEC:
			{
				displayQuiz();
			}
			break;
		case ApplicationState::QUIZ_FINISH:
			{
				displayQuizResults();
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

void Application::displayQuizSelection()
{
	float windowWidth = ImGui::GetContentRegionAvail().x;
	float windowHeight = ImGui::GetContentRegionAvail().y;

	float childWidth = ImGui::GetContentRegionAvail().x * 0.5f;
	float childHeight = 140.0f;

	ImGuiStyle& style = ImGui::GetStyle();
	style.ChildRounding = 10.0f;

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.19f, 0.9f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImColors::VioletHeader);

	ImGui::SetCursorPosY((windowHeight - (childHeight * 2)) * 0.4f);
	ImGui::SetCursorPosX((windowWidth - childWidth) * 0.5f);
	if (ImGui::BeginChild("##questionCountRandom", ImVec2(childWidth, childHeight), ImGuiChildFlags_Borders))
	{
		ImGui::PushFont(bold22);

		float titleWidth = ImGui::CalcTextSize("Random Question Selection").x;
		ImGui::SetCursorPosX((childWidth - titleWidth) * 0.5f);

		ImGui::TextUnformatted("Random Question Selection");
		ImGui::PopFont();

		ImGui::SetCursorPosY((childHeight - 30.0f) * 0.45f);
		ImGui::SetCursorPosX((childWidth - childWidth / 1.3f) * 0.5f);
		ImGui::SetNextItemWidth(childWidth / 1.3f);

		ImGui::InputInt("##Number of Questions", &questionCount);
		ImGui::Spacing();

		titleWidth = ImGui::CalcTextSize("Choose the total number of questions for this quiz").x;
		ImGui::SetCursorPosX((childWidth - titleWidth) * 0.5f);
		ImGui::TextWrapped("Choose the total number of questions for this quiz");

		if (questionCount < 0)
		{
			questionCount = 1;
		}
		else
		{
			questionCount = std::min(questionCount, 1028);
		}
	}
	ImGui::EndChild();

	ImGui::Spacing();
	ImGui::SetCursorPosX((windowWidth - childWidth) * 0.5f);
	if (ImGui::BeginChild("##QuestionInterval", ImVec2(childWidth, childHeight * 1.2f), ImGuiChildFlags_Borders))
	{
		ImGui::PushFont(bold22);

		float titleWidth = ImGui::CalcTextSize("Question Interval").x;
		ImGui::SetCursorPosX((childWidth - titleWidth) * 0.5f);

		ImGui::TextUnformatted("Question Interval");
		ImGui::PopFont();

		ImGui::PushFont(bold18);
		ImGui::Spacing();
		ImGui::TextUnformatted("Interval Start:");

		ImGui::SameLine();
		ImGui::SetCursorPosX(childWidth - childWidth / 2.1f);
		ImGui::TextUnformatted("Interval End:");
		ImGui::PopFont();

		ImGui::SetCursorPosY((childHeight - 30.0f) * 0.65f);
		ImGui::SetNextItemWidth(childWidth / 2.25f);
		ImGui::InputInt("##Start Index", &values[0]);

		ImGui::SameLine();

		ImGui::SetCursorPosX(childWidth - childWidth / 2.1f);
		ImGui::SetNextItemWidth(childWidth / 2.25f);
		ImGui::InputInt("##End Index", &values[1]);

		titleWidth = ImGui::CalcTextSize("Define the interval [Start, End] for test generation").x;
		ImGui::Spacing();
		ImGui::SetCursorPosX((childWidth - titleWidth) * 0.5f);
		ImGui::TextWrapped("%s", "Define the interval [Start, End] for test generation");
	}
	ImGui::EndChild();

	ImGui::Spacing();
	ImGui::SetCursorPosX((windowWidth - (childWidth - childWidth / 2.0f)) * 0.5f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImColors::Violet);
	if (ImGui::Button("Start", ImVec2(childWidth / 2.0f, 30.0f)))
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
	ImGui::PopStyleColor(3);
}

void Application::displayQuiz()
{
	int size = document.root->childrenCount();

	static std::vector<int> selectedResults(ApplicationDefault::maxQuestionOptions, 0);
	std::vector<int> result{};
	std::string statusLabel = std::format("Question {}/{}", currentQuestion + 1, questionCount);

	displayProgressBar(static_cast<float>(currentQuestion) / static_cast<float>(questionCount), ImColors::Turquoise);

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
						ImGui::PushID(resultIdx);
						ImGui::PushStyleColor(ImGuiCol_Header, ImColors::GreenHeader);

						displaySelectableWrap(option->getData(), &selectedResults[resultIdx]);

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

void Application::displayQuizResults()
{
	int size = document.root->childrenCount();

	for (int i = 0; i < selector.size(); ++i)
	{
		int current = selector[i];

		std::string questionTag = document.root->children[current]->getTag();
		std::string questionData = document.root->children[current]->getData();

		if (grade[current] == 1)
		{
			ImGui::PushStyleColor(ImGuiCol_Header, ImColors::GreenHeader);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Header, ImColors::RedHeader);
		}

		if (ImGui::CollapsingHeader(questionData.c_str(), false))
		{
			ImGui::Indent(20.0f);

			int optionIdx{};
			for (const auto& option : document.root->children[current]->children)
			{
				if (optionIdx < ApplicationDefault::maxQuestionOptions
					&& selectionCache[current].size() == ApplicationDefault::maxQuestionOptions
					&& selectionCache[current][optionIdx] == -1)
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
				else if (option->getTag() == "R")
				{
					ImGui::PushStyleColor(ImGuiCol_Header, ImColors::OrangeHeader);
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Header, ImColors::Transparent);
				}

				displaySelectableWrap(option->getData(), nullptr, true);

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
}

bool Application::displaySelectableWrap(const std::string& text, int* condition, bool selected)
{
	float wrapWidth = ImGui::GetContentRegionAvail().x;
	ImVec2 textSize = ImGui::CalcTextSize(text.c_str(), nullptr, false, wrapWidth);
	ImVec2 cursorPos = ImGui::GetCursorPos();

	if (condition != nullptr)
	{
		if (ImGui::Selectable("##", (*condition) != 0, 0, ImVec2(wrapWidth, textSize.y)))
		{
			*condition = ~(*condition);
		}
	}
	else
	{
		ImGui::Selectable("##", selected, 0, ImVec2(wrapWidth, textSize.y));
	}

	ImGui::SetCursorPos(cursorPos);

	ImGui::Indent(10.0f);
	ImGui::TextWrapped("%s", text.c_str());
	ImGui::Unindent(10.0f);

	return (condition != nullptr) ? (*condition) != 0 : selected;
}

void Application::displayProgressBar(float fraction, const ImVec4& color)
{
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);

	ImGui::ProgressBar(fraction, ImVec2(-1.0f, 4.0f));

	ImGui::PopStyleColor();
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

	values[0] = 1;
	values[1] = 1;
	questionCount = ApplicationDefault::minimumQuestionCount;
	currentQuestion = 0;
}

std::vector<int> Application::parseResultLine(const std::string& string)
{
	std::vector<int> result(ApplicationDefault::maxQuestionOptions, 0);

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

	colors[ImGuiCol_Header] = accent;
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
	colors[ImGuiCol_Separator] = accent;

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
