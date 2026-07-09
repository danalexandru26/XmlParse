#pragma once
#include<iostream>
#include<fstream>

#include"XmlNode.h"
#include"XmlParser.h"

enum class State
{
	Default,
	Tag,
	InvalidTag,
	TagClose,
	TagText,
	AttributeTest,
	AttributeRead,
	AttributeValueReadTest,
	AttributeValueRead,
	PotentialAttributeBreak,
	InvalidAttribute = 30
};

class XmlDocument
{
public:
	XmlDocument() = default;
	XmlDocument(std::unique_ptr<XmlNode> root);
	XmlDocument(const XmlDocument& other) = delete;

	XmlNode* findByAttribute(const XmlAttribute& attribute);
	void stream(std::ostream& os);

	void setRoot(std::unique_ptr<XmlNode> node);
	void clear();

	void saveToFile(const std::string& path);
	bool loadFromFile(const std::string& path);

private:
	std::unique_ptr<XmlNode> root;
};
