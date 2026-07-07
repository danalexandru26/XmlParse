#include<iostream>
#include<fstream>
#include"XmlNode.h"
#include"XmlDocument.h"

int main()
{
	using xml = XmlDocument;

	xml document;
	XmlNode root({"p", ""});

	document.setRoot(std::make_unique<XmlNode>(std::move(root)));
	document.stream(std::cout);
	document.saveToFile("testSave.xml");
	return 0;
}
