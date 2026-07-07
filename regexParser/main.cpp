#include<iostream>
#include<fstream>
#include"XmlNode.h"

int main()
{
	XmlNode node("<p>", "");
	XmlNode child("<answer_0>", "a");

	auto pChild = std::make_unique<XmlNode>(std::move(child));
	node.addChild(pChild);

	return 0;
}
