#include"XmlDocument.h"

XmlDoc::XmlDoc(std::unique_ptr<XmlNode>& node)
{
	root = std::move(node);
}

XmlNode* XmlDoc::findByAttribute(const XmlAttribute& attribute)
{
	return root->findByAttribute(attribute);
}
