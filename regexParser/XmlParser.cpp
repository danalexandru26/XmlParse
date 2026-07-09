#include"XmlParser.h"

XmlParser::XmlParser(const std::string& path)
	: path{path}
{
}

int XmlParser::parseToXml(std::function<void(const std::string&)> parser)
{
	if (path.empty())
	{
		return 1;
	}

	return 0;
}

int XmlParser::txtToXml()
{
	if (path.empty())
	{
		return 1;
	}

	std::ifstream toParse(path);
	if (!toParse.is_open())
	{
		std::cerr << "Text file handle cannot be instantiated\n";
		return 1;
	}
}
