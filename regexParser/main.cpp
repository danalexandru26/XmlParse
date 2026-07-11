#include<iostream>
#include<fstream>
#include"XmlNode.h"
#include"XmlDocument.h"
#include"XmlParser.h"

int main()
{
	XmlDocument doc{};
	doc.loadFromFile("xmlSample.xml");

	return 0;
}
