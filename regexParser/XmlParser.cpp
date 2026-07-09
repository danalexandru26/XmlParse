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
		std::cerr << "Path is not set\n";
		return 1;
	}

	std::ifstream toParse(path);
	if (!toParse.is_open())
	{
		std::cerr << "Text file handle cannot be instantiated\n";
		return 1;
	}

	auto state = caseMatch::Default;
	std::string line;
	while (std::getline(toParse, line))
	{
		switch (verifyMatch(line))
		{
		case caseMatch::Question:
			{
				std::cout << std::format("Question: {}\n", line);
			}
			break;
		case caseMatch::Option:
			{
				std::cout << std::format("Option: {}\n", line);
			}
			break;
		case caseMatch::Answer:
			{
				std::cout << std::format("Answer: {}\n", line);
			}
			break;
		case caseMatch::Text:
			{
			}
			break;
		default:
			break;
		}
	}

	return 0;
}

caseMatch XmlParser::verifyMatch(const std::string& line)
{
	static std::regex lineMatch(R"(^\d+\.\D)");
	static std::regex optionMatch(R"(^[a-zA-z][\.\)]\s)");
	static std::regex answerMatch(R"(:.+\()");

	if (std::regex_search(line, lineMatch))
	{
		return caseMatch::Question;
	}
	if (std::regex_search(line, optionMatch))
	{
		return caseMatch::Option;
	}
	if (std::regex_search(line, answerMatch))
	{
		return caseMatch::Answer;
	}
	return caseMatch::Text;
}