#include "wsdlHandler.h"

#include "logUtil.h"
#include "xmlParser.h"
#include "OpcUAClient.h"

#include <typeinfo>

void wsdlHandler::readWsdlFile(std::string fileName)
{

	xmlParser xml = xmlParser();
	xml.initXmlParserGetDocumentGetRootElement(fileName);

	xml.getUrlFromDocument();

	xml.getInput();

	std::string url = xml.getUrl();
	OpcUAClient client;
	client.createAndConnectClient(url);

	std::string opcuaNodeName = xml.getInputOpcUaNodeName();
	std::string value = xml.getInputValue();
	client.writeService(value, opcuaNodeName);
	
	
	// durchsuche file nach porttype
		// suche dort nach output

	// suche nach message mit namen aus output 

	// suche danach in types, schema, das element mit namen aus message

	// nimm dann dort den namen (NodeID) und lese den Wert so lange bis dieser einen fertigen Prozess anzeigt

	client.cleanClient();

	logUtil::writeLogMessageToConsoleAndFile("debug", typeid(wsdlHandler).name(), __LINE__, "Execution of service: " + fileName + " done");
}
