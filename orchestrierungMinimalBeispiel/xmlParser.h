#ifndef XMLPARSER_H

#define XMLPARSER_H

#include <xercesc/dom/DOM.hpp>

#include <string>

using namespace xercesc_3_2;

class xmlParser
{

private:

	DOMElement* rootElement;
	std::string url;
	std::string inputOpcUaNodeName;
	std::string inputValue;

	DOMElement* getElementByNameFromRoot(std::string elementName);
	DOMElement* getChildElementByName(DOMElement* element, std::string elementName);
	std::string getAttributeAndConvertToString(DOMElement* element, std::string attributeName);

public:	

	std::string getUrl();
	std::string getInputOpcUaNodeName();
	std::string getInputValue();

	void initXmlParserGetDocumentGetRootElement(std::string fileName);

	void getUrlFromDocument();

	void getInput();

};

#endif