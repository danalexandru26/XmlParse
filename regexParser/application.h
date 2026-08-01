#pragma once
#include<algorithm>
#include<numeric>
#include<random>
#include"Constants.h"
#include"Renderer.h"
#include"XmlNode.h"
#include"XmlDocument.h"
#include"XmlParser.h"

class Application
{
public:
	Application() = default;
	Application(std::pair<int, int> version, const int& width, const int& height, const std::string& title);

	bool start();
	bool readText(const std::string_view& path);
	bool readXml(const std::string& path);

private:
	bool setupRenderer();

	void displayMainMenuBar();
	void displayDropDown();
	void displayMainWindow();
	void displayStatusWindow();

	void displayQuizSelection();
	void displayQuiz();
	void displayQuizResults();

	void randomizeSet(const int& size);
	void fillSet(const int& start, const int& finish);


	void resetQuizData();

	std::vector<int> parseResultLine(const std::string& string);

	void setLightTheme();
	void setDarkTheme();

private:
	int currentQuestion;
	int questionCount;

	std::array<int, ApplicationDefault::maxQuestionCount> grade;
	std::array<std::vector<int>, ApplicationDefault::maxQuestionCount> selectionCache;
	std::vector<int> selector;
	std::string title;

	ApplicationState state;
	Renderer renderer;
	XmlParser parser;
	XmlDocument document;
};
