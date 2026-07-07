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

	file << XML_HEADER;
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

	uint8_t state = S_DEFAULT;
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
		case S_DEFAULT:
			{
				if (c == '<')
				{
					state = S_TAG;
				}
			}
			break;
		case S_TAG:
			{
				if (isalpha(c))
				{
					tag.push_back(c);
				}
				else if (c == ' ')
				{
					if (tag.empty())
					{
						state = S_INVALID_TAG;
					}
					else
					{
						state = S_ATTRIBUTE_TEST;
					}
				}
				else if (c == '>')
				{
					state = S_TAG_CLOSE;
				}
			}
			break;
		case S_TAG_CLOSE:
			{
				state = S_DEFAULT;
			}
			break;
		case S_INVALID_TAG:
			{
				std::cerr << "Invalid tag syntax\n";
				return false;
			}
			break;
		case S_ATTRIBUTE_TEST:
			{
				if (c == ' ')
				{
				}
				else if (currentAttribute.empty() && isalpha(c))
				{
					currentAttribute.push_back(c);
					state = S_ATTRIBUTE_READ;
				}
				else if (currentAttribute.empty() && isdigit(c))
				{
					state = S_INVALID_ATTRIBUTE;
				}
				else if (c == '>')
				{
					state = S_TAG_CLOSE;
				}
				else
				{
					state = S_INVALID_ATTRIBUTE;
				}
			}
			break;
		case S_ATTRIBUTE_READ:
			{
				if (isalnum(c) || c == '_')
				{
					currentAttribute.push_back(c);
				}
				else if (c == ' ')
				{
					state = S_POTENTIAL_ATTRIBUTE_BREAK;
				}
				else if (c == '=')
				{
					attributes.push_back(currentAttribute);
					currentAttribute.clear();

					state = S_ATTRIBUTE_VALUE_READ_TEST;
				}
			}
			break;
		case S_POTENTIAL_ATTRIBUTE_BREAK:
			{
				if (c == ' ')
				{
				}
				else if (c == '=')
				{
					attributes.push_back(currentAttribute);
					currentAttribute.clear();

					state = S_ATTRIBUTE_VALUE_READ_TEST;
				}
				else
				{
					state = S_INVALID_ATTRIBUTE;
				}
			}
			break;
		case S_INVALID_ATTRIBUTE:
			{
				std::cerr << std::format("Invalid attribute for tag {}\n", tag);
				return false;
			}
			break;

		case S_ATTRIBUTE_VALUE_READ_TEST:
			{
				if (c == '"')
				{
					state = S_ATTRIBUTE_VALUE_READ;
				}
				else if (c != ' ')
				{
					state = S_INVALID_ATTRIBUTE;
				}
			}
			break;
		case S_ATTRIBUTE_VALUE_READ:
			{
				if (c == '"')
				{
					attributeValues.push_back(currentAttributeValue);
					currentAttributeValue.clear();

					state = S_ATTRIBUTE_TEST;
				}
				else
				{
					currentAttributeValue.push_back(c);
				}
			}
			break;
		}

		if (state == S_TAG_CLOSE)
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
