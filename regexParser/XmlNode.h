#pragma once
#include<memory>
#include<vector>
#include<string>
#include<format>
#include<regex>
#include<ostream>
#include"XmlAttribute.h"

class XmlNode
{
public:
	XmlNode() = default;
	XmlNode(const std::string& tag, const std::string& data);

	void setTag(const std::string& val);
	void setData(const std::string& val);
	void setAttribute(const XmlAttribute& val);
	void setAttribute(const std::string& attrib, const std::string& val);

	friend std::ostream& operator<<(std::ostream& os, const XmlNode& node);

private:
	std::string tag;
	std::string data;
	XmlAttribute attribute;
	std::vector<std::unique_ptr<XmlNode>> children;
};
