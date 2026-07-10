#pragma once
#include <iostream>
#include<functional>
#include<memory>
#include<type_traits>
#include<concepts>
#include<vector>
#include<string>
#include<format>
#include<stack>
#include<regex>
#include<ostream>
#include"XmlAttribute.h"

enum class NodeData
{
	Tag = 0x0,
	Data = 0x1,
	Attribute = 0x2,
};

template <typename T>
struct is_bitmask_enum : std::false_type
{
};

template <>
struct is_bitmask_enum<NodeData> : std::true_type
{
};

template <typename T>
concept EnumMask = std::is_enum_v<T> && is_bitmask_enum<T>::value;

template <EnumMask E>
constexpr E operator|(E lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

template <EnumMask E>
constexpr E operator&(E lhs, E rhs)
{
	using T = std::underlying_type_t<E>;
	return static_cast<E>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

class XmlNode
{
public:
	XmlNode() = default;
	XmlNode(const std::string& tag, const std::string& data, const XmlAttribute& attribute = {});
	XmlNode(XmlNode& other) = delete;
	XmlNode(XmlNode&& other) noexcept;

	void setTag(const std::string& val);
	void setData(const std::string& val);
	void setAttribute(const XmlAttribute& val);
	void setAttribute(const std::string& attrib, const std::string& val);

	void addChild(std::unique_ptr<XmlNode> child);
	void addChild(XmlNode&& child);
	void addAttribute(const XmlAttribute& val);

	std::string sanitize();
	std::string getTag();
	std::string getData();
	size_t childCount();

	void appendData(const std::string& val);

	XmlNode* findByAttribute(const XmlAttribute& attrib);
	XmlNode& operator=(XmlNode other);

	friend void print(std::ostream& os, const std::unique_ptr<XmlNode>& root);
	friend void swap(XmlNode& lhs, XmlNode& rhs);

private:
	std::string tag;
	std::string data;
	XmlAttribute attribute;
	std::vector<std::unique_ptr<XmlNode>> children;
};
