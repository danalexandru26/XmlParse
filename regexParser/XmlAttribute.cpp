#include "XmlAttribute.h"

XmlAttribute::XmlAttribute(const std::string& attribute, const std::string& value)
{
	attributes.push_back(attribute);
	values.push_back(value);
}

bool XmlAttribute::addAttribute(const std::string& attribute, const std::string& value)
{
	if (is_valid(attribute))
	{
		attributes.push_back(attribute);
		attributes.push_back(value);

		return true;
	}
	return false;
}

bool XmlAttribute::addAttributes(const std::vector<std::string>& attribute, const std::vector<std::string>& value)
{
	if (attribute.size() != value.size())
	{
		return false;
	}

	for (std::size_t i = 0; i < attribute.size(); ++i)
	{
		if (!is_valid(attribute[i]))
		{
			attributes.clear();
			values.clear();

			return false;
		}
		attributes.push_back(attribute[i]);
		values.push_back(value[i]);
	}
	return true;
}

bool XmlAttribute::is_valid(const std::string& attribute) const
{
	std::regex pattern("^[a-zA-z0-9_]+");

	return std::regex_search(attribute, pattern);
}

bool XmlAttribute::is_valid() const
{
	for (auto& attribute : attributes)
	{
		if (!is_valid(attribute))
		{
			return false;
		}
	}
	return true;
}
