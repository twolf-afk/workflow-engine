#include "engine.h"
// 70
#include "logUtil.h"
#include "wsdlParser.h"
#include "OpcUAClient.h"
#include "indexFileParser.h"
#include "configFileUtil.h"
#include "open62541Util.h"
#include "util.h"

#include <typeinfo>

void Engine::getProcessFromServiceLib(std::string processName) {

	configFileUtil::confParam parameter = configFileUtil::readConfig();
	OpcUAClient client;
	client.createAndConnectClient(parameter.serviceLibURL);

	std::string process = client.callMethod(processName, "ns=1;s=Get Process");

	if (util::stringContainsSubstring(processName, "xml")) {
		processName = util::splitString(processName, ".")[0];
	}

	util::createDirectory(parameter.pathToProcesses + "/" + processName);
	util::saveStringAsFile(process, parameter.pathToProcesses + "/" + processName + "/" + processName + ".xml");

	client.cleanClient();
}
// 62
void Engine::getServicesOfProcessFromServiceLib(std::string processName) {

	std::string directory = processName;
	if (util::stringContainsSubstring(processName, "xml")) {
		directory = util::splitString(processName, ".")[0];
	}
	else {
		processName = processName + ".xml";
	}

	configFileUtil::confParam parameter = configFileUtil::readConfig();
	indexFileParser indexFile = indexFileParser(parameter.pathToProcesses + "/" + directory + "/" + processName);
	std::map<int, std::string> services = indexFile.getServices();

	OpcUAClient client;
	client.createAndConnectClient(parameter.serviceLibURL);

	std::map<int, std::string>::iterator iterator;
	for (iterator = services.begin(); iterator != services.end(); iterator++) {
		std::string serviceName = iterator->second;

		if (util::stringContainsSubstring(processName, "xml")) {
			processName = util::splitString(processName, ".")[0];
		}

		std::string arguments = serviceName + ";" + processName;

		std::string service = client.callMethod(arguments, "ns=1;s=Get Service of Process");

		util::saveStringAsFile(service, parameter.pathToProcesses + "/" + processName + "/" + serviceName);

	}

	client.cleanClient();
}
// 51
void Engine::executeService(std::string processName, std::string serviceName) {

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Start execution of service: " + serviceName);

	if (util::stringContainsSubstring(processName, "xml")) {
		processName = util::splitString(processName, ".")[0];
	}

	configFileUtil::confParam parameter = configFileUtil::readConfig();
	wsdlParser service = wsdlParser();
	service.initXmlParserGetDocumentGetRootElement(parameter.pathToProcesses + processName + "/" + serviceName);

	service.getUrlFromDocument();

	std::string url = service.getUrl();
	OpcUAClient client;
	client.createAndConnectClient(url);
	
	service.getInput();
	std::queue<serviceInput> inputs = service.getInputs();

	size_t inputSize = inputs.size();
	for (size_t i = 0; i < inputSize; i++) {

		serviceInput input = inputs.front();
		inputs.pop();

		if (util::stringContainsSubstring(input.getValue(), "HWD_")) {

			logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "ServiceLink detected");

			std::string value = input.getValue();

			std::vector<std::string> valueParts = util::splitString(value, "-");
			std::string filename = valueParts[0];

			wsdlParser serviceOfOutput = wsdlParser();
			serviceOfOutput.initXmlParserGetDocumentGetRootElement(parameter.pathToProcesses + processName + "/" + filename);
			serviceOfOutput.getUrlFromDocument();
			
			std::string urlOfOutputService = serviceOfOutput.getUrl();
			OpcUAClient clientOutputService;
			clientOutputService.createAndConnectClient(urlOfOutputService);
			
			serviceOfOutput.getOutput();
			std::queue<serviceInput> outputs = serviceOfOutput.getOutputs();
			size_t outputsSize = outputs.size();
			for (size_t i = 0; i < outputsSize; i++) {
				serviceInput output = outputs.front();
				outputs.pop();

				if (output.getNodeId() == valueParts[1]) {

					UA_Variant resultVariant;
					UA_Variant_init(&resultVariant);

					resultVariant = clientOutputService.readService(output);

					std::string strNodeId = input.getNodeId();

					UA_NodeId nodeId = open62541Util::createNodeId(strNodeId);

					logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Value from: " + valueParts[1] + " write it to node: " + strNodeId);
					client.writeValue(nodeId, &resultVariant);
				}

			}
			
		}
		else {
			client.writeService(input);
		}
	}	

	client.createAndConnectClient(url);

	service.getOutput();
	std::queue<serviceInput> outputs = service.getOutputs();
	serviceInput processDoneArgument;
	
	size_t outputSize = outputs.size();
	for (size_t i = 0; i < outputSize; i++) {

		serviceInput output = outputs.front();
		outputs.pop();

		if (util::stringContainsSubstring(output.getNodeId(), "processDone")) {
			processDoneArgument = output;
		}
	}

	bool processDone = false;
	
	while (!processDone) {

		UA_Variant result;
		UA_Variant_init(&result);

		result = client.readService(processDoneArgument);

		processDone = open62541Util::convertUaVariantToBool(result);

		Sleep(5000);
	}

	client.cleanClient();

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Execution of service: " + serviceName + " done");
}
// 14
void Engine::executeProcess(std::string processName) {

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Execute process: " + processName);

	if (util::stringContainsSubstring(processName, "xml")) {
		processName = util::splitString(processName, ".")[0];
	}

	configFileUtil::confParam parameter = configFileUtil::readConfig();
	indexFileParser indexFile = indexFileParser(parameter.pathToProcesses + processName + "/" + processName + ".xml");
	std::map<int, std::string> services = indexFile.getServices();

	std::map<int, std::string>::iterator iterator;
	for (iterator = services.begin(); iterator != services.end(); iterator++) {
		std::string serviceName = iterator->second;

		executeService(processName, serviceName);
	}

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Execution of process done");
}

void Engine::executeProcessStep(std::string processName, int sequenceNumber) {

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Execute process step");

	if (util::stringContainsSubstring(processName, "xml")) {
		processName = util::splitString(processName, ".")[0];
	} 

	configFileUtil::confParam parameter = configFileUtil::readConfig();
	indexFileParser indexFile = indexFileParser(parameter.pathToProcesses + processName + "/" + processName + ".xml");
	std::map<int, std::string> services = indexFile.getServices();

	std::string serviceName = services.at(sequenceNumber);

	executeService(processName, serviceName);

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(Engine).name(), __LINE__, "Execution of process done");
}