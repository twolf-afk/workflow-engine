#include "engine.h"

#include "logUtil.h"
#include "wsdlParser.h"
#include "OpcUAClient.h"
#include "indexFileParser.h"
#include "configFileUtil.h"
#include "util.h"

#include <typeinfo>

void Engine::executeService(std::string serviceName)
{
	logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Start execution of service: " + serviceName);

	configFileUtil::confParam parameter = configFileUtil::readConfig();

	wsdlParser serviceWsdl = wsdlParser();
	serviceWsdl.initXmlParserGetDocumentGetRootElement(parameter.pathToServices + serviceName);

	serviceWsdl.getUrlFromDocument();

	serviceWsdl.getInput();

	std::string url = serviceWsdl.getUrl();
	OpcUAClient client;
	client.createAndConnectClient(url);
	
	std::queue<serviceInput> inputs = serviceWsdl.getInputs();
	client.writeService(inputs);
	

	serviceWsdl.getOutput();
	std::queue<serviceInput> outputs = serviceWsdl.getOutputs();
	
	bool processDone = false;

	while (!processDone)
	{
		processDone = client.readService(outputs);

		Sleep(5000);

	}

	client.writeService(outputs);

	client.cleanClient();

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Execution of service: " + serviceName + " done");
}

void Engine::executeProcess(std::string processName)
{
	logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Execute process: " + processName);

	indexFileParser indexFile = indexFileParser(processName);
	std::map<int, std::string> services = indexFile.getServices();

	// OpcUAClient client;
	// client.createAndConnectClient("opc.tcp://DESKTOP-0AULV4D:2/");

	// configFileUtil::confParam parameter = configFileUtil::readConfig();

	std::map<int, std::string>::iterator iterator;
	for (iterator = services.begin(); iterator != services.end(); iterator++)
	{
		std::string serviceName = iterator->second;

		// std::string service = client.callMethod(serviceName, "ns=1;s=Get Service");

		// util::saveStringAsFile(service, parameter.pathToServices + serviceName);

		executeService(serviceName);
	}
	// client.cleanClient();

}