#pragma once
#include<fstream>
#include<string>
#include<format>
#include<unordered_map>

class XmlParser
{
public:
	XmlParser() = default;
	XmlParser(const std::string& path);
	~XmlParser();

private:
	std::ofstream file;
};
