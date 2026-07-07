#pragma once
#include<iostream>
#include<fstream>
#include"XmlNode.h"

class XmlDocument
{
public:
	XmlDocument() = default;
	XmlDocument(std::unique_ptr<XmlNode> root);
	XmlDocument(const XmlDocument& other) = delete;

	XmlNode* findByAttribute(const XmlAttribute& attribute);
	void stream(std::ostream& os);

private:
	std::unique_ptr<XmlNode> root;
};
