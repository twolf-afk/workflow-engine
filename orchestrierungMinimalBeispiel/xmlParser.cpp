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

std::string xmlParser::getOutputOpcUaNodeName()
{
    return outputOpcUaNodeName;
}

std::string xmlParser::getOutputValue()
{
    return outputValue;
}

void xmlParser::setOutputValue(std::string value)
{
    outputValue = value;
}

// TODO index gibt das element an, welches aus der Liste ausgewaehlt wird
// kann zu problemen fuehren, wenn sich die Reihenfolge in der xml aendert
xercesc_3_2::DOMElement* xmlParser::getElementByNameAndIndexFromElement(std::string elementTagName, int index, xercesc_3_2::DOMElement* element)
{

    XMLCh* tageName = xercesc_3_2::XMLString::transcode(elementTagName.c_str());
    xercesc_3_2::DOMNodeList* nodes = element->getElementsByTagName(tageName);
    xercesc_3_2::DOMNode* node = nodes->item(index);
    xercesc_3_2::DOMElement* childElement = dynamic_cast<xercesc::DOMElement*>(node);

    return childElement;
}

std::string xmlParser::getAttributeAndConvertToString(xercesc_3_2::DOMElement* element, std::string attributeName)
{

    XMLCh* xmlChAttributeName = xercesc_3_2::XMLString::transcode(attributeName.c_str());
    const XMLCh* xmlChAttributeValue = element->getAttribute(xmlChAttributeName);

    return xercesc_3_2::XMLString::transcode(xmlChAttributeValue);
}

void xmlParser::initXmlParserGetDocumentGetRootElement(std::string fileName)
{
    xercesc_3_2::XMLPlatformUtils::Initialize();

    // TODO eigene Exception schreiben
    // TODO aufruf von try und catch block in eigene funktionen auslagern
    try
    {
        xercesc_3_2::XercesDOMParser* parser = new xercesc_3_2::XercesDOMParser();
        parser->setValidationScheme(xercesc_3_2::XercesDOMParser::Val_Always);
        parser->setDoNamespaces(true);    // optional

        parser->parse(fileName.c_str());
        
        // TODO Wenn das dokument gar nicht im selben verzeichnis ist, dann kann es auch nicht geparst werden, also müsste es eine Fehlermeldung geben
        xercesc_3_2::DOMDocument* xmlDoc = parser->getDocument();
        rootElement = xmlDoc->getDocumentElement();
        
    }
    catch (xercesc::XMLException& e)
    {
        const XMLCh* xmlChErrorMessage = e.getMessage();
        std::string errorMessage = xercesc_3_2::XMLString::transcode(xmlChErrorMessage);

        logUtil::writeLogMessageToConsoleAndFile("debug", typeid(xmlParser).name(), __LINE__, errorMessage);

    }
}

void xmlParser::getUrlFromDocument()
{

    xercesc_3_2::DOMElement* soapElement = getElementByNameAndIndexFromElement("soap:address", 0, rootElement);

    url = getAttributeAndConvertToString(soapElement, "location");

    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(xmlParser).name(), __LINE__, "Set URL to: " + url);
    
}

void xmlParser::getInput()
{
    xercesc_3_2::DOMElement* inputElement = getElementByNameAndIndexFromElement("element", 1, rootElement);

    inputOpcUaNodeName = getAttributeAndConvertToString(inputElement, "name");
    inputValue = getAttributeAndConvertToString(inputElement, "value");

    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(xmlParser).name(), __LINE__, "OPC UA input node: " + inputOpcUaNodeName);
    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(xmlParser).name(), __LINE__, "Input value: " + inputValue);
}

void xmlParser::getOutput()
{
    xercesc_3_2::DOMElement* outputElement = getElementByNameAndIndexFromElement("element", 3, rootElement);

    outputOpcUaNodeName = getAttributeAndConvertToString(outputElement, "name");

    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(xmlParser).name(), __LINE__, "OPC UA output node: " + outputOpcUaNodeName);
}