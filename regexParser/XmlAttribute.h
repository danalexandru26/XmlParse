#pragma once
#include<vector>
#include<string>
#include <regex>

class XmlAttribute
{
public:
	XmlAttribute() = default;
	XmlAttribute(const std::string& attribute, const std::string& value);

	bool addAttribute(const std::string& attribute, const std::string& value);
	bool addAttributes(const std::vector<std::string>& attribute, const std::vector<std::string>& value);

	bool is_valid() const;
	bool is_valid(const std::string& attribute) const;

	std::vector<std::string> attributes;
	std::vector<std::string> values;
};
