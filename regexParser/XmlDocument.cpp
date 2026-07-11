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
	//iterate(os, root);
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

	auto state = State::Default;

	std::string tag{};
	std::string endTag{};
	std::vector<std::string> attributes{};
	std::vector<std::string> attributeValues{};

	std::string currentAttribute{};
	std::string currentAttributeValue{};
	std::string data{};

	std::unique_ptr<XmlNode> child{};
	std::unique_ptr<XmlNode> root = std::make_unique<XmlNode>("root");
	std::stack<XmlNode*> stack{};

	char c{};
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
				if (c == '/')
				{
					state = State::NodeClosure;
				}
				else
				{
					if (isalpha(c) || c == '_')
					{
						tag.push_back(c);
					}
					else if (isdigit(c) && !tag.empty())
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
						XmlNode* parent{nullptr};
						if (!stack.empty())
						{
							parent = stack.top();
						}
						child = std::make_unique<XmlNode>(tag);
						XmlNode* childPtr = child.get();

						if (parent != nullptr)
						{
							parent->addChild(std::move(child));
						}
						else
						{
							root->addChild(std::move(child));
						}
						stack.push(childPtr);
						state = State::NodeData;
						tag.clear();
						state = State::NodeData;
					}
				}
			}
			break;
		case State::TagClose:
			{
				if (c == ' ')
				{
				}
				if (c == '<')
				{
					state = State::NodeClosure;
				}
				else
				{
					state = State::InvalidNode;
				}
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
					XmlNode* parent{nullptr};
					if (!stack.empty())
					{
						parent = stack.top();
					}
					child = std::make_unique<XmlNode>(tag);
					XmlNode* childPtr = child.get();

					if (parent != nullptr)
					{
						parent->addChild(std::move(child));
					}
					else
					{
						root->addChild(std::move(child));
					}
					stack.push(childPtr);
					tag.clear();
					state = State::NodeData;
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

					currentAttribute.clear();
					currentAttributeValue.clear();

					state = State::AttributeTest;
				}
				else
				{
					currentAttributeValue.push_back(c);
				}
			}
			break;
		case State::NodeData:
			{
				if (c == '<')
				{
					if (!stack.empty())
					{
						XmlNode* current = stack.top();
						current->appendData(data);

						data.clear();
					}
					state = State::Tag;
				}
				else
				{
					data.push_back(c);
				}
			}
			break;
		case State::NodeClosure:
			{
				if ((isalpha(c) || c == '_'))
				{
					endTag.push_back(c);
				}
				else if (c == ' ')
				{
					if (endTag.empty())
					{
						state = State::InvalidTag;
					}
				}
				else if (c == '>')
				{
					if (!stack.empty())
					{
						stack.pop();
					}
					endTag.clear();
					state = State::Default;
				}
			}
			break;
		}
	}
	print(std::cout, root);
	return true;
}
