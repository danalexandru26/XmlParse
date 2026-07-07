#pragma once
#include<iostream>
#include<fstream>

#include"XmlNode.h"
#include"XmlParser.h"

#define XML_HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"

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
	void loadFromFile(const std::string& path);

private:
	std::unique_ptr<XmlNode> root;
};
