#include "OpcUAClient.h"

#include "logUtil.h"

#include <open62541/client_config_default.h>

#include <stdio.h>

bool OpcUAClient::createAndConnectClient(std::string url)
{
    
    client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));
    
    UA_StatusCode statusCode = UA_Client_connect(client, url.c_str());
    
    if (statusCode != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        logUtil::writeLogMessageToConsoleAndFile("debug", typeid(OpcUAClient).name(), __LINE__, "Error, could not create and connect client");
        logUtil::writeLogMessageToConsoleAndFile("debug", typeid(OpcUAClient).name(), __LINE__, "Error code: " + statusCode);
        return false;
    }
    else
    {
        logUtil::writeLogMessageToConsoleAndFile("debug", typeid(OpcUAClient).name(), __LINE__, "Create and connect client successful");
        return true;
    }
}

bool OpcUAClient::writeStringToNode(std::string value, std::string identifier)
{
    char *chIdentifier = new char[identifier.length() + 1];
    strcpy(chIdentifier, identifier.c_str());

    UA_Variant variant;
    UA_Variant_setScalar(&variant, &value, &UA_TYPES[UA_TYPES_STRING]);

    UA_StatusCode statusCode = UA_Client_writeValueAttribute(client, UA_NODEID_STRING(1, chIdentifier), &variant);
    UA_Variant_clear(&variant);

    if (statusCode != UA_STATUSCODE_GOOD) {
        logUtil::writeLogMessageToConsoleAndFile("debug", typeid(OpcUAClient).name(), __LINE__, "Error, could not write value: " + value + " to identifier: " + identifier);
        logUtil::writeLogMessageToConsoleAndFile("debug", typeid(OpcUAClient).name(), __LINE__, "Error code: " + statusCode);
        return false;
    }
    else
    {
        logUtil::writeLogMessageToConsoleAndFile("debug", typeid(OpcUAClient).name(), __LINE__, "Writing successful");
        return true;
    }

    return statusCode;

}