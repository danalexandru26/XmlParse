#pragma once
#include<string>

class XmlAttribute
{
public:
	XmlAttribute() = default;
	XmlAttribute(const std::string& attribute, const std::string& value);

	std::string attribute;
	std::string value;
};
