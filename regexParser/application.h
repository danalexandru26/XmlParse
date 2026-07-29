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

const std::string defaultPathText = R"(D:\github\XmlParse\x64\Debug\questions.txt)";
constexpr std::array<float, 4> defaultColor{0.1f, 0.1f, 0.1f, 1.0f};


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

	void setLightTheme();
	void setDarkTheme();

	void randomizeSet(const int& size);
	std::vector<int> parseResultLine(const std::string& string);

private:
	std::array<int, 1028> grade;
	std::vector<int> selector;
	std::string title;

	ApplicationState state;
	Renderer renderer;
	XmlParser parser;
	XmlDocument document;
};
