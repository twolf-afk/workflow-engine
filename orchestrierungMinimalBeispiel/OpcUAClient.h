#ifndef OPCUACLIENT_H

#define OPCUACLIENT_H

#include <open62541/client.h>
#include <open62541/client_highlevel.h>

#include <string>

class OpcUAClient
{

private:

	UA_Client* client;

	void writeValue(UA_NodeId nodeID, UA_Variant variant);
	std::string readValue(UA_NodeId nodeID, UA_Variant variant);

public:


	bool createAndConnectClient(std::string url);

	void writeService(std::string value, std::string identifier);

	std::string readService(std::string identifier);

	void cleanClient();

};

#endif

