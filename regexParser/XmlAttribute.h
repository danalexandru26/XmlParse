#pragma once
#include<string>
#include <regex>

class XmlAttribute
{
public:
	XmlAttribute() = default;
	XmlAttribute(const std::string& attribute, const std::string& value);

	bool is_valid() const;

	friend bool operator==(const XmlAttribute& lhs, const XmlAttribute& rhs);
	friend bool operator!=(const XmlAttribute& lhs, const XmlAttribute& rhs);

	std::string attribute;
	std::string value;
};
