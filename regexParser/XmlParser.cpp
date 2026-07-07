#include"XmlParser.h"

XmlParser::XmlParser(const std::string& path)
{
	file.open(path);
}

XmlParser::~XmlParser()
{
	if (file.is_open())
	{
		file.close();
	}
}

bool XmlParser::validateTag(const std::string& tag)
{
	std::regex regTag("^<(?!\\d)[a-zA-Z0-9_]+>");

	return std::regex_search(tag, regTag);
}

bool XmlParser::validateData(const std::string& data)
{
	std::regex regTag("^[a-zA-z0-9_]+");

	return std::regex_search(data, regTag);
}
