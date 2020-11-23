#include "wsdlParser.h"

#include "logUtil.h"

#include <xercesc/parsers/XercesDOMParser.hpp>

#include <iostream>

std::string wsdlParser::getUrl()
{
    return url;
}

std::queue<serviceInput> wsdlParser::getInputs()
{
    return inputs;
}

std::queue<serviceInput> wsdlParser::getOutputs()
{
    return outputs;
}

// TODO index gibt das element an, welches aus der Liste ausgewaehlt wird
// kann zu problemen fuehren, wenn sich die Reihenfolge in der xml aendert
xercesc_3_2::DOMElement* wsdlParser::getElementByNameAndIndexFromElement(std::string elementTagName, int index, xercesc_3_2::DOMElement* element)
{

    XMLCh* tagName = xercesc_3_2::XMLString::transcode(elementTagName.c_str());
    xercesc_3_2::DOMNodeList* nodes = element->getElementsByTagName(tagName);
    xercesc_3_2::DOMNode* node = nodes->item(index);
    xercesc_3_2::DOMElement* childElement = dynamic_cast<xercesc::DOMElement*>(node);

    return childElement;
}

std::string wsdlParser::getAttributeAndConvertToString(xercesc_3_2::DOMElement* element, std::string attributeName)
{

    XMLCh* xmlChAttributeName = xercesc_3_2::XMLString::transcode(attributeName.c_str());
    const XMLCh* xmlChAttributeValue = element->getAttribute(xmlChAttributeName);

    return xercesc_3_2::XMLString::transcode(xmlChAttributeValue);
}

void wsdlParser::initXmlParserGetDocumentGetRootElement(std::string fileName)
{
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(wsdlParser).name(), __LINE__, "Initialization of xml parser");
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(wsdlParser).name(), __LINE__, "Filename: " + fileName);

    xercesc_3_2::XMLPlatformUtils::Initialize();

    // TODO eigene Exception schreiben
    // TODO aufrufe in try catch block in eigene funktionen auslagern
    try
    {
        xercesc_3_2::XercesDOMParser* parser = new xercesc_3_2::XercesDOMParser();
        parser->setValidationScheme(xercesc_3_2::XercesDOMParser::Val_Always);
        parser->setDoNamespaces(true);

        parser->parse(fileName.c_str());
        
        // TODO Wenn das dokument gar nicht im selben verzeichnis ist, dann kann es auch nicht geparst werden, also müsste es eine Fehlermeldung geben
        xercesc_3_2::DOMDocument* xmlDoc = parser->getDocument();
        rootElement = xmlDoc->getDocumentElement();
        
    }
    catch (xercesc::XMLException& e)
    {
        const XMLCh* xmlChErrorMessage = e.getMessage();
        std::string errorMessage = xercesc_3_2::XMLString::transcode(xmlChErrorMessage);

        logUtil::writeLogMessageToConsoleAndFile("debug", typeid(wsdlParser).name(), __LINE__, errorMessage);

    }
}

void wsdlParser::getUrlFromDocument()
{
    
    xercesc_3_2::DOMElement* soapElement = getElementByNameAndIndexFromElement("soap:address", 0, rootElement);

    url = getAttributeAndConvertToString(soapElement, "location");

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(wsdlParser).name(), __LINE__, "Set URL to: " + url);
    
}

serviceInput wsdlParser::getArgumentsFromWsdl(xercesc_3_2::DOMElement* element, int i)
{
    xercesc_3_2::DOMElement* element = getElementByNameAndIndexFromElement("element", i, element);

    serviceInput serviceInputArguments = serviceInput();

    std::string nodeId = getAttributeAndConvertToString(element, "name");
    serviceInputArguments.setNodeId(nodeId);

    std::string datatype = getAttributeAndConvertToString(element, "type");
    serviceInputArguments.setDatatype(datatype);

    std::string value = getAttributeAndConvertToString(element, "value");
    serviceInputArguments.setValue(value);

    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(wsdlParser).name(), __LINE__, "nodeID: " + nodeId);
    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(wsdlParser).name(), __LINE__, "datatype: " + datatype);
    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(wsdlParser).name(), __LINE__, "value: " + value);
}

void wsdlParser::getInput()
{
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(wsdlParser).name(), __LINE__, "Parse input information");

    xercesc_3_2::DOMElement* inputElements = getElementByNameAndIndexFromElement("all", 0, rootElement);
    size_t childsCount = inputElements->getChildElementCount();
    
    for (size_t i = 0; i < childsCount; i++)
    {
        serviceInput argument = wsdlParser::getArgumentsFromWsdl(inputElements, i);

        inputs.push(argument);
    }
}

void wsdlParser::getOutput()
{
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(wsdlParser).name(), __LINE__, "Parse output information");

    xercesc_3_2::DOMElement* inputElements = getElementByNameAndIndexFromElement("all", 1, rootElement);
    int childsCount = inputElements->getChildElementCount();

    for (int i = 0; i < childsCount; i++)
    {
        serviceInput argument = wsdlParser::getArgumentsFromWsdl(inputElements, i);

        outputs.push(argument);
    }
}