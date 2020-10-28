#ifndef XMLPARSER_H

#define XMLPARSER_H

#include <xercesc/dom/DOM.hpp>

#include <string>

class xmlParser
{

private:

	xercesc_3_2::DOMElement* rootElement;
	std::string url;
	std::string inputOpcUaNodeName;
	std::string inputValue;

	xercesc_3_2::DOMElement* getElementByNameFromRoot(std::string elementName);
	xercesc_3_2::DOMElement* getChildElementByName(xercesc_3_2::DOMElement* element, std::string elementName);
	std::string getAttributeAndConvertToString(xercesc_3_2::DOMElement* element, std::string attributeName);

public:	

	std::string getUrl();
	std::string getInputOpcUaNodeName();
	std::string getInputValue();

	void initXmlParserGetDocumentGetRootElement(std::string fileName);

	void getUrlFromDocument();

	void getInput();

};

#endif