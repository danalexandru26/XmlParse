#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<regex>
#include<vector>
#include<functional>
#include"XmlNode.h"


enum class caseMatch
{
	Default,
	Question,
	Option,
	Answer,
	Text
};

class XmlParser
{
public:
	XmlParser() = default;
	XmlParser(const std::string& path);

	void configurePath(const std::string& path);
	int parseToXml(std::function<void(const std::string&)> parser);
	int txtToXml();

private:
	caseMatch classifyState(const std::string& line);
	std::smatch extractText(const std::regex& pattern, const std::string& line);

	const std::regex lineMatch{R"(^(\d+)\.\D)"};
	const std::regex optionMatch{R"(^([a-zA-z])[\.\)]\s)"};
	const std::regex answerMatch{R"(:.+\()"};
	std::string path;
};
