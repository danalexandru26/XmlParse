#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<format>
#include<regex>
#include<vector>
#include<functional>

class XmlParser
{
public:
	XmlParser() = default;
	XmlParser(const std::string& path);

	void configurePath(const std::string& path);
	int parseToXml(std::function<void(const std::string &)> parser);
	int txtToXml();

private:
	std::string path;
};
