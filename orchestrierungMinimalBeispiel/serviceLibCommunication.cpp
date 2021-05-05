#include "serviceLibCommunication.h"

#include "util.h"
#include "configFileUtil.h"
#include "OpcUAClient.h"
#include "open62541Util.h"

#include <string>

void serviceLibCommunication::saveServicesInServiceLib() {

	configFileUtil::confParam parameter = configFileUtil::readConfig();
	std::vector<std::string> services = util::getFilesInDirectory(parameter.pathToServices);

	size_t numberOfServices = services.size();

	OpcUAClient client = OpcUAClient();
	client.createAndConnectClient(parameter.serviceLibURL);

	for (size_t i = 0; i < numberOfServices; i++) {

		std::string service = services.at(i);

		std::string serviceText = util::getFile(parameter.pathToServices + "/" + service);

		std::string nodeId = "ns=1;s=Save Service as WSDL";

		client.callMethod(service + "," + serviceText, nodeId);
	}

	client.cleanClient();
}