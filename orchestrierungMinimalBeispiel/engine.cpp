#include "engine.h"

#include "logUtil.h"
#include "wsdlParser.h"
#include "OpcUAClient.h"
#include "indexFileParser.h"
#include "configFileUtil.h"

#include <typeinfo>

void Engine::executeService(std::string serviceName)
{
	logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Start execution of service: " + serviceName);

	configFileUtil::confParam parameter = configFileUtil::readConfig();

	// TODO "xml" umbennen
	wsdlParser xml = wsdlParser();
	xml.initXmlParserGetDocumentGetRootElement(parameter.pathToServices + serviceName);

	// TODO delete "FromDocument"
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

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Execution of service: " + serviceName + " done");
}

void Engine::executeProcess(std::string processName)
{
	logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Execute process: " + processName);

	indexFileParser indexFile = indexFileParser(processName); // "indexfile.xml"
	std::map<int, std::string> processMap = indexFile.getProcess();

	std::map<int, std::string>::iterator it;

	for (it = processMap.begin(); it != processMap.end(); it++)
	{
		executeService(it->second);
	}


}