#pragma once
#include<array>
#include<algorithm>
#include<numeric>
#include<random>
#include"Renderer.h"
#include"XmlNode.h"
#include"XmlDocument.h"
#include"XmlParser.h"

constexpr int defaultWidth = 1280;
constexpr int defaultHeight = 720;

constexpr int maxQuestionCount = 1028;
constexpr int maxQuestionOptions = 6;

const std::string defaultPathText = R"(D:\github\XmlParse\x64\Debug\questions.txt)";
constexpr std::array<float, 4> defaultColor{0.1f, 0.1f, 0.1f, 1.0f};

struct ImVec4;

namespace ImColors
{
	constexpr ImVec4 Red = ImVec4(0.85f, 0.25f, 0.25f, 1.0f);
	constexpr ImVec4 Green = ImVec4(0.18f, 0.65f, 0.32f, 1.0f);
	constexpr ImVec4 RedHeader = ImVec4(0.90f, 0.20f, 0.20f, 0.70f);
	constexpr ImVec4 GreenHeader = ImVec4(0.15f, 0.75f, 0.30f, 0.70f);
	constexpr ImVec4 Transparent = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
}

enum class ApplicationState
{
	QUIZ_SELECT,
	QUIZ_EXEC,
	QUIZ_FINISH
};

class Application
{
public:
	Application() = default;
	Application(std::pair<int, int> version, const int& width, const int& height, const std::string& title);

	bool start();
	bool readText(const std::string& path);
	bool readXml(const std::string& path);

private:
	bool setupRenderer();

	void displayMainMenuBar();
	void displayDropDown();
	void displayMainWindow();
	void displayStatusWindow();

	void displayQuizSelection(int& questionCount);
	void displayQuiz(int& questionCount);
	void displayQuizResults(int& questionCount);

	void randomizeSet(const int& size);
	void fillSet(const int& start, const int& finish);

	void resetQuizData();

	std::vector<int> parseResultLine(const std::string& string);

	void setLightTheme();
	void setDarkTheme();

private:
	std::array<int, maxQuestionCount> grade;
	std::array<std::vector<int>, maxQuestionCount> selectionCache;
	std::vector<int> selector;
	std::string title;

	ApplicationState state;
	Renderer renderer;
	XmlParser parser;
	XmlDocument document;
};
