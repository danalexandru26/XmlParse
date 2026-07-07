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
