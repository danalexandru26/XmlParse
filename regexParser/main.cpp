#include<iostream>
#include<fstream>
#include"XmlNode.h"

int main()
{
	XmlNode root;
	XmlNode node("<p>", "");
	XmlNode child("<p1>", "a");
	XmlNode childOfChild("<p2>", "correct");

	auto pChildOfChild = std::make_unique<XmlNode>(std::move(childOfChild));
	node.addChild(pChildOfChild);

	auto pChild = std::make_unique<XmlNode>(std::move(child));
	node.addChild(pChild);

	auto start = std::make_unique<XmlNode>(std::move(node));
	root.addChild(start);

	iterate(std::cout, root);

	return 0;
}
