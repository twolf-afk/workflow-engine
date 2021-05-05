#include "indexFileParser.h"

#include "logUtil.h"

#include <xercesc/parsers/XercesDOMParser.hpp>

using namespace xercesc_3_2;

indexFileParser::indexFileParser(std::string filename) {

    initXmlParserGetDocumentGetRootElement(filename);
}

// TODO diese funktion ist ein zu eins gleich mit der aus wsdlParser -> dopplung -> nicht gut
void indexFileParser::initXmlParserGetDocumentGetRootElement(std::string fileName) {

    XMLPlatformUtils::Initialize();

    // TODO eigene Exception schreiben
    // TODO aufruf von try und catch block in eigene funktionen auslagern
    try {
        XercesDOMParser* parser = new XercesDOMParser();
        parser->setValidationScheme(XercesDOMParser::Val_Always);
        parser->setDoNamespaces(true);    // optional

        parser->parse(fileName.c_str());

        // TODO Wenn das dokument gar nicht im selben verzeichnis ist, dann kann es auch nicht geparst werden, also müsste es eine Fehlermeldung geben
        DOMDocument* xmlDoc = parser->getDocument();
        rootElement = xmlDoc->getDocumentElement();

    } catch (xercesc::XMLException& e) {
        const XMLCh* xmlChErrorMessage = e.getMessage();
        std::string errorMessage = XMLString::transcode(xmlChErrorMessage);

        logUtil::writeLogMessageToConsoleAndFile("error", typeid(indexFileParser).name(), __LINE__, errorMessage);
    }
}

std::map<int, std::string> indexFileParser::getServices() {

    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(indexFileParser).name(), __LINE__, "Get Services from process");

    std::map<int, std::string> process;
    
    XMLCh* tagName = XMLString::transcode("service");

    DOMNodeList* services = rootElement->getElementsByTagName(tagName);
    const XMLSize_t serviceCount = services->getLength();

    for (XMLSize_t  i = 0; i < serviceCount; i++) {

        DOMNode* node = services->item(i);
        DOMElement* currentService = dynamic_cast<DOMElement*>(node);
        
        XMLCh* attributeName = XMLString::transcode("sequenceNumber");
        const XMLCh* xmlChsequenceNumber = currentService->getAttribute(attributeName);
        std::string strSequenceNumber = XMLString::transcode(xmlChsequenceNumber);
        int sequenceNumber = std::stoi(strSequenceNumber);

        XMLCh* linkTagName = XMLString::transcode("link");
        DOMNodeList* links = currentService->getElementsByTagName(linkTagName);
        DOMNode* link = links->item(0);
        DOMElement* linkElement = dynamic_cast<DOMElement*>(link);

        const XMLCh* xmlChText = link->getTextContent();
        std::string text = XMLString::transcode(xmlChText);

        process[sequenceNumber] = text;
    }

    return process;
}