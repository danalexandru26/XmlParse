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

	parseXml.txtToXml();

	return 0;
}
