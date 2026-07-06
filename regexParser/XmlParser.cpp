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
