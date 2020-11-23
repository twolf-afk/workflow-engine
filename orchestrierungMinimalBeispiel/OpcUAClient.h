#ifndef OPCUACLIENT_H

#define OPCUACLIENT_H

#include "serviceInput.h"

#include <open62541/client.h>
#include <open62541/client_highlevel.h>

#include <string>
#include <queue>

class OpcUAClient
{

private:

	UA_Client* client;

	void writeValue(UA_NodeId nodeID, UA_Variant* variant);
	bool readValue(UA_NodeId nodeID, UA_Variant variant);

public:


	void createAndConnectClient(std::string url);

	void writeService(std::queue<serviceInput> inputs);

	bool readService(std::queue<serviceInput> outputs);

	std::string callMethod(std::string inputArgument, std::string strNodeId);

	void cleanClient();

};

#endif

