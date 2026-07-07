#include"XmlDocument.h"

XmlDocument::XmlDocument(std::unique_ptr<XmlNode> node)
{
	root = std::move(node);
}

XmlNode* XmlDocument::findByAttribute(const XmlAttribute& attribute)
{
	return root->findByAttribute(attribute);
}

void XmlDocument::stream(std::ostream& os)
{
	iterate(os, root);
}

void XmlDocument::setRoot(std::unique_ptr<XmlNode> node)
{
	if (root == nullptr)
	{
		root = std::make_unique<XmlNode>(XmlNode({"root", ""}));
	}
	root->addChild(std::move(node));
}

void XmlDocument::clear()
{
	if (root != nullptr)
	{
	}
}

void XmlDocument::saveToFile(const std::string& path)
{
	std::ofstream file;
	file.open(path);

	if (!file.is_open())
	{
		std::cerr << "Writing to file cannot be performed\n";
		return;
	}

	file << XML_HEADER;
	iterate(file, root);
}

void XmlDocument::loadFromFile(const std::string& path)
{
}
