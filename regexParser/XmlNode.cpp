#include"XmlNode.h"


XmlNode::XmlNode(const std::string& tag, const std::string& data, const XmlAttribute& attribute)
	: tag{tag}, data{data}, attribute{attribute}
{
}


XmlNode::XmlNode(XmlNode&& other) noexcept
{
	swap(*this, other);
}

void XmlNode::setTag(const std::string& val)
{
	std::regex tagTest("^<(?!\\d)[a-zA-Z0-9_]+>");

	if (std::regex_search(val, tagTest))
	{
		tag = val;
	}
}

void XmlNode::setData(const std::string& val)
{
	data = val;
}

void XmlNode::setAttribute(const XmlAttribute& val)
{
	attribute = val;
}

void XmlNode::setAttribute(const std::string& attrib, const std::string& val)
{
	std::regex attribPattern("^[a-zA-z0-9_]+");
	if (std::regex_search(attrib, attribPattern) &&
		std::regex_search(attrib, attribPattern))
	{
		attribute = XmlAttribute(attrib, val);
	}
}

std::string XmlNode::getTag()
{
	return tag;
}

std::string XmlNode::getData()
{
	return data;
}

size_t XmlNode::childCount()
{
	return children.size();
}

void XmlNode::appendData(const std::string& val)
{
	data.append(val);
}

void XmlNode::addChild(std::unique_ptr<XmlNode> child)
{
	children.push_back(std::move(child));
}

void XmlNode::addChild(XmlNode&& child)
{
	children.push_back(std::make_unique<XmlNode>(std::move(child)));
}

void XmlNode::addAttribute(const XmlAttribute& val)
{
	attribute = val;
}

XmlNode* XmlNode::findByAttribute(const XmlAttribute& attrib)
{
	if (!attrib.is_valid())
	{
		return nullptr;
	}

	std::stack<XmlNode*> stack{};

	for (auto& child : children)
	{
		stack.push(child.get());
	}

	while (!stack.empty())
	{
		XmlNode* top = stack.top();
		if (top->attribute == attrib)
		{
			return top;
		}

		stack.pop();
		for (auto& child : top->children)
		{
			stack.push(child.get());
		}
	}
	return nullptr;
}


XmlNode& XmlNode::operator=(XmlNode other)
{
	swap(*this, other);

	return *this;
}

void swap(XmlNode& lhs, XmlNode& rhs)
{
	using std::swap;

	swap(lhs.tag, rhs.tag);
	swap(lhs.data, rhs.data);
	swap(lhs.attribute, rhs.attribute);

	lhs.children.swap(rhs.children);
}

void print(std::ostream& os, const std::unique_ptr<XmlNode>& root)
{
	if (root->children.empty())
	{
		std::cerr << "Root is empty\n";
		return;
	}
	os << ' ';
	if (!os)
	{
		std::cerr << "Output operation failure\n";
		return;
	}

	std::stack<std::pair<XmlNode*, bool>> stack{};
	stack.push({root.get(), false});

	while (!stack.empty())
	{
		auto [node, visited] = stack.top();
		stack.pop();

		if (visited)
		{
			os << std::format("</{}>\n", node->tag);
		}
		else
		{
			std::string attribute{};
			if (node->attribute.is_valid())
			{
				attribute = node->attribute.stream();
			}

			os << std::format("<{}{}>\n", node->tag, attribute);
			if (!node->data.empty())
			{
				std::string data = node->sanitize();
				os << std::format("{}\n", data);
			}

			stack.push({node, true});
			for (auto& child : node->children)
			{
				if (child != nullptr)
				{
					stack.push({child.get(), false});
				}
			}
		}
	}
}

std::string XmlNode::sanitize()
{
	std::string clean{data};

	for (size_t i = 0; i < clean.length(); ++i)
	{
		switch (clean[i])
		{
		case '>':
			{
				clean.replace(i, 1, R"(&gt;)");
				i += 1;
			}
			break;
		case '<':
			{
				clean.replace(i, 1, R"(&lt;)");
				i += 1;
			}
			break;
		case '&':
			{
				clean.replace(i, 1, R"(&amp;)");
				i += 1;
			}
			break;
		default:
			break;
		}
	}

	return clean;
}
