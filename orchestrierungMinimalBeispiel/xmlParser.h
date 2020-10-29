#ifndef XMLPARSER_H

#define XMLPARSER_H

#include <xercesc/dom/DOM.hpp>

#include <string>

class xmlParser
{

private:

	xercesc_3_2::DOMElement* rootElement;
	std::string url;

	// TODO es gibt Dienste mit mehreren Variablen und Inputs
	// diese variablen hier in eine Liste von inputs und outputs aendern
	std::string inputOpcUaNodeName;
	std::string inputValue;
	std::string outputOpcUaNodeName;
	std::string outputValue;

	xercesc_3_2::DOMElement* getElementByNameAndIndexFromElement(std::string elementTagName, int index, xercesc_3_2::DOMElement* element);
	std::string getAttributeAndConvertToString(xercesc_3_2::DOMElement* element, std::string attributeName);

public:	

	std::string getUrl();
	std::string getInputOpcUaNodeName();
	std::string getInputValue();
	std::string getOutputOpcUaNodeName();
	std::string getOutputValue();

	void setOutputValue(std::string value);

	void initXmlParserGetDocumentGetRootElement(std::string fileName);

	void getUrlFromDocument();

	void getInput();

	void getOutput();

};

#endif