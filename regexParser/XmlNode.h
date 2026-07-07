#pragma once
#include<memory>
#include<vector>
#include<string>
#include<format>
#include<stack>
#include<regex>
#include<ostream>
#include"XmlAttribute.h"

class XmlNode
{
public:
	XmlNode() = default;
	XmlNode(const std::string& tag, const std::string& data);
	XmlNode(XmlNode& other) = delete;
	XmlNode(XmlNode&& other) noexcept;

	void setTag(const std::string& val);
	void setData(const std::string& val);
	void setAttribute(const XmlAttribute& val);
	void setAttribute(const std::string& attrib, const std::string& val);

	void addChild(std::unique_ptr<XmlNode>& child);
	void addChild(XmlNode&& child);
	void addAttribute(const XmlAttribute& val);

	XmlNode* findByAttribute(const XmlAttribute& attrib);

	XmlNode& operator=(XmlNode other);

	friend void swap(XmlNode& lhs, XmlNode& rhs);
	friend void iterate(std::ostream& os, const XmlNode& node);
	friend std::ostream& operator<<(std::ostream& os, const XmlNode& node);

private:
	std::string tag;
	std::string data;
	XmlAttribute attribute;
	std::vector<std::unique_ptr<XmlNode>> children;
};
