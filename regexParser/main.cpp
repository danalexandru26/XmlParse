#include<iostream>
#include<fstream>
#include"XmlNode.h"
#include"XmlDocument.h"

int main()
{
	XmlNode root;
	XmlNode node("p", "");
	XmlNode child("p1", "a");
	XmlNode childOfChild("p2", "correct");


	node.setAttribute("question", "0");
	node.addChild(child);
	node.addChild(childOfChild);

	root.addChild(node);

	iterate(std::cout, root);
	return 0;
}
