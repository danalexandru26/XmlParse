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

void XmlNode::addChild(std::unique_ptr<XmlNode>& child)
{
	children.push_back(std::move(child));
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

std::ostream& operator<<(std::ostream& os, const XmlNode& node)
{
	std::string closingTag = node.tag;
	closingTag.insert(1, "/");
	os << node.tag << '\n';

	for (const auto& child : node.children)
	{
		std::string closingTag = child->tag;
		closingTag.insert(1, "/");

		auto format = std::format("{}{}{}\n", child->tag, child->data, closingTag);
		os << format;
	}
	os << closingTag;

	return os;
}
