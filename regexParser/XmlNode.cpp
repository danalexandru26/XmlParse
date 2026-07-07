#include"XmlNode.h"

XmlNode::XmlNode(const std::string& tag, const std::string& data)
	: tag{tag}, data{data}
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

void XmlNode::addChild(std::unique_ptr<XmlNode>& child)
{
	children.push_back(std::move(child));
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

void iterate(std::ostream& os, const XmlNode& root)
{
	if (root.children.empty())
	{
		return;
	}

	std::stack<std::pair<XmlNode*, bool>> stack{};
	stack.push({root.children[0].get(), false});

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
				attribute = std::format(" {} = {}",
				                        node->attribute.attribute, node->attribute.value);
			}

			os << std::format("<{}{}>\n", node->tag, attribute);

			if (!node->data.empty())
			{
				os << node->data << '\n';
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

std::ostream& operator<<(std::ostream& os, const XmlNode& node)
{
	std::string closingTag = node.tag;
	closingTag.insert(1, "/");

	std::string format = std::format("{}\n{}\n{}\n", node.tag, node.data, closingTag);
	os << format;
	return os;
}
