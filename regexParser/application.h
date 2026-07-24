#pragma once
#include"XmlNode.h"
#include"XmlDocument.h"
#include"XmlParser.h"

enum class Mode : uint32_t
{
	Console = 1,
	Graphical = 2,
};

class Application
{
public:
	Application() = default;
	Application(const std::string title, Mode mode);

	int readText(const std::string& path);
	int readXml(const std::string& path);

private:
	std::string title;
	Mode mode;

	XmlParser parser;
	XmlDocument document;
};
