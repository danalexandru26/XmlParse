#pragma once
#include<fstream>
#include<string>
#include<format>

class XmlParser
{
public:
	XmlParser() = default;
	XmlParser(const std::string& path);
	~XmlParser();

private:
	std::ofstream file;
};
