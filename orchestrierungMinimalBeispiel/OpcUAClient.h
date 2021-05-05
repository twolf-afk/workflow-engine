#ifndef OPCUACLIENT_H

#define OPCUACLIENT_H

#include "serviceInput.h"

#include <open62541/client.h>
#include <open62541/client_highlevel.h>

#include <string>
#include <queue>

class OpcUAClient {

private:

	UA_Client* client;

public:

	void writeValue(UA_NodeId nodeID, UA_Variant* variant);

	UA_Variant readValue(UA_NodeId nodeID, UA_Variant variant);

	void createAndConnectClient(std::string url);

	UA_BrowseResponse browseService(std::string nodeId);

	void writeService(serviceInput inputs);

	UA_Variant readService(serviceInput outputs);

	std::string callMethod(std::string inputArgument, std::string strNodeId);

	void cleanClient();

};

#endif