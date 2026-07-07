#pragma once
#include<fstream>
#include<string>
#include<format>
#include<regex>

class XmlParser
{
public:
	XmlParser() = default;
	XmlParser(const std::string& path);
	~XmlParser();

	static bool validateTag(const std::string& tag);
	static bool validateData(const std::string& data);

private:
	std::ofstream file;
};
