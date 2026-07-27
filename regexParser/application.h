#pragma once
#include<array>
#include"Renderer.h"
#include"XmlNode.h"
#include"XmlDocument.h"
#include"XmlParser.h"

constexpr int defaultWidth = 1280;
constexpr int defaultHeight = 720;

constexpr int fallbackOpenglMajor = 3;
constexpr int fallbackOpenglMinor = 3;

constexpr std::array<float, 4> defaultColor{0.1f, 0.1f, 0.1f, 1.0f};

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
	void displayDefaultScreen();

private:
	std::string title;

	Renderer renderer;
	XmlParser parser;
	XmlDocument document;
};
