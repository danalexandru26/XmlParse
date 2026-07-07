#include<iostream>
#include<fstream>
#include"XmlNode.h"
#include"XmlDocument.h"

int main()
{
	using xml = XmlDocument;

	xml document;
	document.loadFromFile("xmlSample.xml");

	return 0;
}
