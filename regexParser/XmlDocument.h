#pragma once
#include<iostream>
#include<fstream>
#include"XmlNode.h"

class XmlDoc
{
public:
	XmlDoc() = default;
	XmlDoc(std::unique_ptr<XmlNode>& root);
	XmlDoc(const XmlDoc& other) = delete;

	XmlNode* findByAttribute(const XmlAttribute& attribute);
	void iterate(std::ostream& os);

private:
	std::unique_ptr<XmlNode> root;
};
