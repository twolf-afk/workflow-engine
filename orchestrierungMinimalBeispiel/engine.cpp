#include "engine.h"

#include "logUtil.h"
#include "wsdlParser.h"
#include "OpcUAClient.h"
#include "indexFileParser.h"

#include <typeinfo>

void Engine::executeService(std::string fileName)
{
	indexFileParser indexFile = indexFileParser("indexfile.xml");
	std::map<int, std::string> process = indexFile.getProcess();

	wsdlParser xml = wsdlParser();
	xml.initXmlParserGetDocumentGetRootElement(fileName);

	xml.getUrlFromDocument();

	xml.getInput();

	std::string url = xml.getUrl();
	OpcUAClient client;
	client.createAndConnectClient(url);


	std::string inputNode = xml.getInputOpcUaNodeName();
	std::string valueToWrite = xml.getInputValue();
	client.writeService(valueToWrite, inputNode);
	

	xml.getOutput();
	std::string outputNode = xml.getOutputOpcUaNodeName();
	
	std::string result = client.readService(outputNode);
	
	// nimm dann dort den namen (NodeID) und lese den Wert so lange bis dieser einen fertigen Prozess anzeigt

	client.cleanClient();

	logUtil::writeLogMessageToConsoleAndFile("debug", typeid(Engine).name(), __LINE__, "Execution of service: " + fileName + " done");
}
