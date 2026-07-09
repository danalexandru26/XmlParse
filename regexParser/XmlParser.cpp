#include"XmlParser.h"
#include <cstdio>
#include <windows.h>

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

	auto state{caseMatch::Default};
	auto lineState{caseMatch::Default};

	while (std::getline(toParse, line))
	{
		if (std::regex_search(line, lineMatch))
		{
			lineState = caseMatch::Question;
		}
		else if (std::regex_search(line, optionMatch))
		{
			lineState = caseMatch::Option;
		}
		else if (std::regex_search(line, answerMatch))
		{
			lineState = caseMatch::Answer;
		}
		else
		{
			lineState = caseMatch::Text;
		}

		switch (state)
		{
		case caseMatch::Default:
			{
				switch (lineState)
				{
				case caseMatch::Question:
					{
						auto match = extractText(lineMatch, line);
						if (!match.empty() && match.size() > 1)
						{
							questionNode = std::make_unique<XmlNode>("q_" + match[1].str(), line);
							state = caseMatch::Question;
						}
						else
						{
						}
					}
					break;
				default:
					{
						std::cerr << std::format("Invalid pattern. Skipping line: {}\n", line);
					}
				}
			}
			break;
		case caseMatch::Question:
			{
				switch (lineState)
				{
				case caseMatch::Option:
					{
						auto match = extractText(optionMatch, line);
						if (!match.empty() && match.size() > 1)
						{
							optionNode = std::make_unique<XmlNode>(match[1].str(), line);
							state = caseMatch::Option;
						}
						else
						{
						}
					}
					break;
				case caseMatch::Text:
					{
						if (questionNode != nullptr)
						{
							questionNode->appendData(" " + line);
						}
					}
					break;
				default:
					{
						std::cerr << "Invalid document formatting for question text or option identification\n";
						return 1;
					}
				}
			}
			break;
		case caseMatch::Option:
			{
				switch (lineState)
				{
				case caseMatch::Option:
					{
						if (questionNode != nullptr)
						{
							if (questionNode != nullptr)
							{
								questionNode->addChild(std::move(optionNode));
							}

							auto match = extractText(optionMatch, line);
							if (!match.empty() && match.size() > 1)
							{
								optionNode = std::make_unique<XmlNode>(match[1].str(), line);
							}
							else
							{
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
							if (optionNode != nullptr)
							{
								questionNode->addChild(std::move(optionNode));
							}
							nodes.push_back(std::move(questionNode));
							state = caseMatch::Default;
						}
					}
					break;
				case caseMatch::Text:
					{
						if (optionNode != nullptr)
						{
							optionNode->appendData(" " + line);
						}
					}
					break;
				default:
					{
						std::cerr << "Invalid document formatting for option text or answer identification\n";
						return 1;
					}
				}
			}
			break;
		default:
			{
				std::cerr << "Invalid document formatting\n";
				return 1;
			}
		}
	}

	SetConsoleOutputCP(65001);
	for (auto& ptr : nodes)
	{
		iterate(std::cout, ptr);
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
