#include"XmlParser.h"

XmlParser::XmlParser(const std::string& path)
	: path{path}
{
}

void XmlParser::configurePath(const std::string& path)
{
	if (this->path.empty())
	{
		this->path = path;
	}
}

int XmlParser::parseToXml(std::function<void(const std::string&)> parser)
{
	if (path.empty())
	{
		return 1;
	}
	return 0;
}

std::unique_ptr<XmlNode> XmlParser::txtToXml()
{
	if (path.empty())
	{
		std::cerr << "Path is not set\n";
		return nullptr;
	}

	std::ifstream toParse(path);
	if (!toParse.is_open())
	{
		std::cerr << "Text file handle cannot be instantiated\n";
		return nullptr;
	}

	std::unique_ptr<XmlNode> root = std::make_unique<XmlNode>("root", "");
	std::unique_ptr<XmlNode> questionNode{};
	std::unique_ptr<XmlNode> optionNode{};

	auto state{caseMatch::Default};
	auto lineState{caseMatch::Default};

	std::string line{};
	while (std::getline(toParse, line))
	{
		lineState = classifyState(line);

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
						std::cerr << line<< "Invalid document formatting for question text or option identification\n";
						return nullptr;
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
							if (optionNode != nullptr)
							{
								questionNode->addChild(std::move(optionNode));
							}

							auto match = extractText(optionMatch, line);
							if (!match.empty() && match.size() > 1)
							{
								optionNode = std::make_unique<XmlNode>(match[1].str(), line);
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

							auto answer = std::make_unique<XmlNode>("R", line);
							questionNode->addChild(std::move(answer));
							root->addChild(std::move(questionNode));

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
						std::cerr << line<<"Invalid document formatting for option text or answer identification\n";
						return nullptr;
					}
				}
			}
			break;
		default:
			{
				std::cerr << "Invalid document formatting\n";
				return nullptr;
			}
		}
	}

	return
		root;
}

caseMatch XmlParser::classifyState(const std::string& line)
{
	if (std::regex_search(line, lineMatch)) { return caseMatch::Question; }
	if (std::regex_search(line, optionMatch)) { return caseMatch::Option; }
	if (std::regex_search(line, answerMatch)) { return caseMatch::Answer; }

	return caseMatch::Text;
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
