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
	std::unique_ptr<XmlNode> answerNode{};

	auto state = caseMatch::Default;
	int qMatch{};

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
			qMatch += 1;
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
				std::string id = extractText(lineMatch, line);
				questionNode = std::make_unique<XmlNode>(id, "");
			}
			break;
		case caseMatch::Option:
			{
				if (questionNode.get() != nullptr)
				{
					std::string option = extractText(optionMatch, line);
					questionNode->addChild(std::make_unique<XmlNode>(option, ""));
				}
			}
			break;
		case caseMatch::Answer:
			{
				if (questionNode.get() != nullptr)
				{
					nodes.push_back(std::move(questionNode));
				}
			}
			break;
		case caseMatch::Text:
			{
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

std::string XmlParser::extractText(const std::regex& pattern, const std::string& line)
{
	auto begin = std::sregex_iterator(line.begin(), line.end(), pattern);

	for (auto it = begin; it != std::sregex_iterator(); ++it)
	{
		auto match = *it;
		if (match.size() > 1)
		{
			return match[1];
		}
	}
	return {};
}
