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
	std::regex regTag("<\\s*(/)?\\s*([A-Za-z_][A-Za-z0-9_.:-]*)");

	return std::regex_search(tag, regTag);
}
