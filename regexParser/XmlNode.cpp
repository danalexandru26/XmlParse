#include"XmlNode.h"

XmlNode::XmlNode(const std::string& tag, const std::string& data)
	: tag{tag}, data{data}
{
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

std::ostream& operator<<(std::ostream& os, const XmlNode& node)
{
	std::string closingTag = node.tag;
	closingTag.insert(1, "/");

	auto format = std::format("{}{}{}\n", node.tag, node.data, closingTag);
	os << format;

	return os;
}
