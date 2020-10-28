#ifndef OPCUACLIENT_H

#define OPCUACLIENT_H

#include <open62541/client.h>
#include <open62541/client_highlevel.h>

#include <string>

class OpcUAClient
{

private:

	UA_Client* client;

	UA_StatusCode writeValue(UA_NodeId nodeID, UA_Variant variant);

public:


	bool createAndConnectClient(std::string url);

	void writeService(std::string value, std::string nodeID);

	void cleanClient();

};

#endif

