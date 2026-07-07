#pragma once
#include<iostream>
#include<fstream>

#include"XmlNode.h"
#include"XmlParser.h"

#define XML_HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"

#define S_DEFAULT 0
#define S_TAG 1
#define S_INVALID_TAG 2
#define S_TAG_CLOSE 3
#define S_TAG_TEXT 4
#define S_ATTRIBUTE_TEST 5
#define S_ATTRIBUTE_READ 6
#define S_ATTRIBUTE_VALUE_READ_TEST 7
#define S_ATTRIBUTE_VALUE_READ 8
#define S_POTENTIAL_ATTRIBUTE_BREAK 9

#define S_INVALID_ATTRIBUTE 30

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
