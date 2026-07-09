#include"XmlDocument.h"

XmlDocument::XmlDocument(std::unique_ptr<XmlNode> node)
{
	root = std::move(node);
}

XmlNode* XmlDocument::findByAttribute(const XmlAttribute& attribute)
{
	return root->findByAttribute(attribute);
}

void XmlDocument::stream(std::ostream& os)
{
	iterate(os, root);
}

void XmlDocument::setRoot(std::unique_ptr<XmlNode> node)
{
	if (root == nullptr)
	{
		root = std::make_unique<XmlNode>(XmlNode({"root", ""}));
	}
	root->addChild(std::move(node));
}

void XmlDocument::clear()
{
	if (root != nullptr)
	{
	}
}

void XmlDocument::saveToFile(const std::string& path)
{
	std::ofstream file;
	file.open(path);

	if (!file.is_open())
	{
		std::cerr << "Writing to file cannot be performed\n";
		return;
	}
	iterate(file, root);
}

bool XmlDocument::loadFromFile(const std::string& path)
{
	std::ifstream file;
	file.open(path);

	if (!file.is_open())
	{
		std::cerr << "Could not open file\n";
		return false;
	}

	State state = State::Default;
	char c{};

	std::string tag{};
	std::vector<std::string> attributes{};
	std::vector<std::string> attributeValues{};

	std::string currentAttribute{};
	std::string currentAttributeValue{};
	std::string data{};

	std::vector<std::unique_ptr<XmlNode>> nodes{};

	while (file.get(c))
	{
		switch (state)
		{
		case State::Default:
			{
				if (c == '<')
				{
					state = State::Tag;
				}
			}
			break;
		case State::Tag:
			{
				if (isalpha(c))
				{
					tag.push_back(c);
				}
				else if (c == ' ')
				{
					if (tag.empty())
					{
						state = State::InvalidTag;
					}
					else
					{
						state = State::AttributeTest;
					}
				}
				else if (c == '>')
				{
					state = State::TagClose;
				}
			}
			break;
		case State::TagClose:
			{
				state = State::Default;
			}
			break;
		case State::InvalidTag:
			{
				std::cerr << "Invalid tag syntax\n";
				return false;
			}
			break;
		case State::AttributeTest:
			{
				if (c == ' ')
				{
				}
				else if (currentAttribute.empty() && isalpha(c))
				{
					currentAttribute.push_back(c);
					state = State::AttributeRead;
				}
				else if (currentAttribute.empty() && isdigit(c))
				{
					state = State::InvalidAttribute;
				}
				else if (c == '>')
				{
					state = State::TagClose;
				}
				else
				{
					state = State::InvalidAttribute;
				}
			}
			break;
		case State::AttributeRead:
			{
				if (isalnum(c) || c == '_')
				{
					currentAttribute.push_back(c);
				}
				else if (c == ' ')
				{
					state = State::PotentialAttributeBreak;
				}
				else if (c == '=')
				{
					attributes.push_back(currentAttribute);
					currentAttribute.clear();

					state = State::AttributeValueReadTest;
				}
			}
			break;
		case State::PotentialAttributeBreak:
			{
				if (c == ' ')
				{
				}
				else if (c == '=')
				{
					attributes.push_back(currentAttribute);
					currentAttribute.clear();

					state = State::AttributeValueReadTest;
				}
				else
				{
					state = State::InvalidAttribute;
				}
			}
			break;
		case State::InvalidAttribute:
			{
				std::cerr << std::format("Invalid attribute for tag {}\n", tag);
				return false;
			}
			break;

		case State::AttributeValueReadTest:
			{
				if (c == '"')
				{
					state = State::AttributeValueRead;
				}
				else if (c != ' ')
				{
					state = State::InvalidAttribute;
				}
			}
			break;
		case State::AttributeValueRead:
			{
				if (c == '"')
				{
					attributeValues.push_back(currentAttributeValue);
					currentAttributeValue.clear();

					state = State::AttributeTest;
				}
				else
				{
					currentAttributeValue.push_back(c);
				}
			}
			break;
		}

		if (state == State::TagClose)
		{
			auto node = std::make_unique<XmlNode>(tag, "");
			nodes.push_back(std::move(node));
		}
	}

	for (auto& node : nodes)
	{
		std::cout << node->getTag() << '\n';
	}
}
