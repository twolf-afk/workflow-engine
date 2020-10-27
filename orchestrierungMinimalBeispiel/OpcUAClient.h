#ifndef OPCUACLIENT_H

#define OPCUACLIENT_H

#include <open62541/client.h>
#include <open62541/client_highlevel.h>

#include <string>

class OpcUAClient
{

private:

	UA_Client* client;;

public:


	bool createAndConnectClient(std::string url);

	bool writeStringToNode(std::string value, std::string nodeID);

};

#endif

