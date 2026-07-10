#include<iostream>
#include<fstream>
#include"XmlNode.h"
#include"XmlDocument.h"
#include"XmlParser.h"

int main()
{
	using xml = XmlDocument;
	using parser = XmlParser;

	parser parseXml("fixedQuiz.txt");

	std::unique_ptr<XmlNode> root = parseXml.txtToXml();

	std::ofstream test;
	print(test, root);
	return 0;
}
