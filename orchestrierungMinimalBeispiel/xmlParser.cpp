#include "xmlParser.h"

#include "logUtil.h"

#include <xercesc/parsers/XercesDOMParser.hpp>

#include <iostream>

std::string xmlParser::getUrl()
{
    return url;
}

std::string xmlParser::getInputOpcUaNodeName()
{
    return inputOpcUaNodeName;
}

std::string xmlParser::getInputValue()
{
    return inputValue;
}

// TODO es wird immer das erste Element zurueck gegeben -> variable gestalten
DOMElement* xmlParser::getElementByNameFromRoot(std::string elementName)
{

    XMLCh* tageName = XMLString::transcode(elementName.c_str());
    DOMNodeList* nodes = rootElement->getElementsByTagName(tageName);
    DOMNode* node = nodes->item(0);
    DOMElement* element = dynamic_cast<xercesc::DOMElement*>(node);

    return element;
}

// TODO es wird immer das erste Element zurueck gegeben -> variable gestalten
DOMElement* xmlParser::getChildElementByName(DOMElement* element, std::string elementName)
{

    XMLCh* tageName = XMLString::transcode(elementName.c_str());
    DOMNodeList* nodes = element->getElementsByTagName(tageName);
    DOMNode* node = nodes->item(0);
    DOMElement* childElement = dynamic_cast<xercesc::DOMElement*>(node);

    return childElement;
}

std::string xmlParser::getAttributeAndConvertToString(DOMElement* element, std::string attributeName)
{

    XMLCh* xmlChAttributeName = XMLString::transcode(attributeName.c_str());
    const XMLCh* xmlChAttributeValue = element->getAttribute(xmlChAttributeName);

    return XMLString::transcode(xmlChAttributeValue);
}

void xmlParser::initXmlParserGetDocumentGetRootElement(std::string fileName)
{
    XMLPlatformUtils::Initialize();

    try
    {
        XercesDOMParser* parser = new XercesDOMParser();
        parser->setValidationScheme(XercesDOMParser::Val_Always);
        parser->setDoNamespaces(true);    // optional

        parser->parse(fileName.c_str());
        
        // TODO Wenn das dokument gar nicht im selben verzeichnis ist, dann kann es auch nicht geparst werden, also müsste es eine Fehlermeldung geben
        DOMDocument* xmlDoc = parser->getDocument();
        rootElement = xmlDoc->getDocumentElement();
        
    }
    catch (xercesc::XMLException& e)
    {
        const XMLCh* xmlChErrorMessage = e.getMessage();
        std::string errorMessage = XMLString::transcode(xmlChErrorMessage);

        logUtil::writeLogMessageToConsoleAndFile("debug", typeid(xmlParser).name(), __LINE__, errorMessage);

    }
}

void xmlParser::getUrlFromDocument()
{

    DOMElement* soapElement = getElementByNameFromRoot("soap:address");

    url = getAttributeAndConvertToString(soapElement, "location");

    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(xmlParser).name(), __LINE__, "Set URL to: " + url);
    
}

void xmlParser::getInput()
{

    DOMElement* all = getElementByNameFromRoot("all");
    DOMElement* inputElement = getChildElementByName(all, "element");

    inputOpcUaNodeName = getAttributeAndConvertToString(inputElement, "name");
    inputValue = getAttributeAndConvertToString(inputElement, "value");

    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(xmlParser).name(), __LINE__, "OPC UA input node: " + inputOpcUaNodeName);
    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(xmlParser).name(), __LINE__, "Input value: " + inputValue);

}