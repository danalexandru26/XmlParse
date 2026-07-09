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

	static std::regex lineMatch(R"(^(\d+)\.\D)");
	static std::regex optionMatch(R"(^([a-zA-z])[\.\)]\s)");
	static std::regex answerMatch(R"(:.+\()");

	std::string line{};

	std::vector<std::unique_ptr<XmlNode>> nodes{};
	std::unique_ptr<XmlNode> questionNode{};
	std::unique_ptr<XmlNode> optionNode{};

	auto state = caseMatch::Default;

	while (std::getline(toParse, line))
	{
		if (std::regex_search(line, lineMatch))
		{
			state = caseMatch::Question;
		}
		else if (std::regex_search(line, optionMatch))
		{
			state = caseMatch::Option;
		}
		else if (std::regex_search(line, answerMatch))
		{
			state = caseMatch::Answer;
		}
		else
		{
			state = caseMatch::Text;
		}

		switch (state)
		{
		case caseMatch::Question:
			{
				auto match = extractText(lineMatch, line);
				if (!match.empty() && match.size() > 1)
				{
					questionNode = std::make_unique<XmlNode>(match[1].str(), "");
				}
				else
				{
				}
			}
			break;
		case caseMatch::Option:
			{
				if (questionNode != nullptr)
				{
					auto match = extractText(lineMatch, line);
					if (!match.empty() && match.size() > 1)
					{
						questionNode->addChild(std::make_unique<XmlNode>(match[1].str(), ""));
					}
				}
				else
				{
				}
			}
			break;
		case caseMatch::Answer:
			{
				if (questionNode != nullptr)
				{
					nodes.push_back(std::move(questionNode));
				}
			}
			break;
		case caseMatch::Text:
			{
				std::cout << line << '\n';
			}
			break;
		case caseMatch::Default:
			{
			}
			break;
		default:
			break;
		}
	}

	return 0;
}

std::smatch XmlParser::extractText(const std::regex& pattern, const std::string& line)
{
	std::smatch match;
	if (std::regex_search(line, match, pattern))
	{
		return match;
	}
	return {};
}
