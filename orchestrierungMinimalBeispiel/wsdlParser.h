#ifndef XMLPARSER_H

#define XMLPARSER_H

#include "serviceInput.h"

#include <xercesc/dom/DOM.hpp>

#include <string>
#include <queue>

class wsdlParser
{

private:

	xercesc_3_2::DOMElement* rootElement;
	std::string url;

	std::queue<serviceInput> inputs;
	std::queue<serviceInput> outputs;

	xercesc_3_2::DOMElement* getElementByNameAndIndexFromElement(std::string elementTagName, int index, xercesc_3_2::DOMElement* element);
	std::string getAttributeAndConvertToString(xercesc_3_2::DOMElement* element, std::string attributeName);

	serviceInput getArgumentsFromWsdl(xercesc_3_2::DOMElement* element, int i);

public:	

	std::string getUrl();
	std::queue<serviceInput> getInputs();
	std::queue<serviceInput> getOutputs();

	void initXmlParserGetDocumentGetRootElement(std::string fileName);

	void getUrlFromDocument();

	void getInput();

	void getOutput();

};

#endif