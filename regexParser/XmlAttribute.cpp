#include "XmlAttribute.h"

XmlAttribute::XmlAttribute(const std::string& attribute, const std::string& value)
	: attribute{attribute}, value{value}
{
}

bool XmlAttribute::is_valid() const
{
	std::regex attribPattern("^[a-zA-z0-9_]+");

	return std::regex_search(attribute, attribPattern) && std::regex_search(value, attribPattern);
}

bool operator==(const XmlAttribute& lhs, const XmlAttribute& rhs)
{
	return lhs.attribute == rhs.attribute && lhs.value == rhs.value;
}

bool operator!=(const XmlAttribute& lhs, const XmlAttribute& rhs)
{
	return lhs.attribute != rhs.attribute && lhs.value != rhs.value;
}
