#pragma once
#include<array>
#include<string_view>

struct ImVec4;
struct ImFont;

namespace ImColors
{
	constexpr ImVec4 Red = ImVec4(0.9f, 0.25f, 0.25f, 1.0f);
	constexpr ImVec4 Green = ImVec4(0.18f, 0.85f, 0.32f, 1.0f);
	constexpr ImVec4 Orange = ImVec4(1.00f, 0.42f, 0.00f, 1.00f);
	constexpr ImVec4 Grey = ImVec4(0.12f, 0.15f, 0.18f, 1.00f);
	constexpr ImVec4 Violet = ImVec4(0.53f, 0.38f, 0.96f, 1.0f);
	constexpr ImVec4 Turquoise = ImVec4(0.21f, 0.74f, 0.60f, 1.0f);

	constexpr ImVec4 RedHeader = ImVec4(0.95f, 0.20f, 0.20f, 0.70f);
	constexpr ImVec4 GreenHeader = ImVec4(0.15f, 0.95f, 0.30f, 0.70f);
	constexpr ImVec4 OrangeHeader = ImVec4(1.00f, 0.42f, 0.00f, 0.70f);
	constexpr ImVec4 GreyHeader = ImVec4(0.12f, 0.15f, 0.18f, 0.70f);
	constexpr ImVec4 VioletHeader = ImVec4(0.53f, 0.38f, 0.96f, 0.7f);

	constexpr ImVec4 Transparent = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
}

namespace ApplicationDefault
{
	constexpr int defaultWidth{1280};
	constexpr int defaultHeight{720};
	constexpr int maxQuestionCount{1028};
	constexpr int maxQuestionOptions{6};
	constexpr int minimumQuestionCount{1};
	constexpr std::array defaultColor{0.0f, 0.0f, 0.0f, 0.0f};
}

namespace SystemPath
{
	constexpr std::string_view defaultFont = R"(C:\Windows\Fonts\segoeuib.ttf)";
	constexpr std::string_view defaultPathText = R"(assets\questions.txt)";
}

enum class ApplicationState
{
	QUIZ_SELECT,
	QUIZ_EXEC,
	QUIZ_FINISH
};
