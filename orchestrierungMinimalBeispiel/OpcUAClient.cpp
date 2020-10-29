#include "OpcUAClient.h"

#include "logUtil.h"
#include "util.h"
#include "OpcUAException.h"

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

void OpcUAClient::writeValue(UA_NodeId nodeID, UA_Variant variant)
{
    UA_StatusCode statusCode = UA_Client_writeValueAttribute(client, nodeID, &variant);

    if (statusCode != UA_STATUSCODE_GOOD)
    {
        throw OpcUAException(statusCode);
    }

}

std::string OpcUAClient::readValue(UA_NodeId nodeID, UA_Variant variant)
{
    UA_StatusCode statusCode = UA_Client_readValueAttribute(client, nodeID, &variant);

    if (statusCode != UA_STATUSCODE_GOOD)
    {
        throw OpcUAException(statusCode);
    }
    else
    {
        UA_Boolean uaResult = *(UA_Boolean*) variant.data;

        std::string result = util::boolToString(uaResult);

        return result;
    }
}

void OpcUAClient::writeService(std::string value, std::string identifier)
{
    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(OpcUAClient).name(), __LINE__, "Writing value: " + value + " to node: ns=2;s=" + identifier);
    
    try
    {
        // TODO der identifier der nodeID kann auch bytestring, guid o.ä. sein -> auslagern in eigene Methoden
        // TODO namespaceIndex variable gestalten
        char* chIdentifier = util::stringToChar(identifier);
        const UA_NodeId nodeID = UA_NODEID_STRING(2, chIdentifier);

        // TODO neben in16 gibt es noch viele weitere datentypen, hierzu jeweils eine methode erstelleb, ggf. mit vererbung, abstraktion, interfaces etc.
        UA_Variant variant;
        UA_Variant_setScalar(&variant, &value, &UA_TYPES[UA_TYPES_BOOLEAN]);

        writeValue(nodeID, variant);
        
    }
    catch (OpcUAException& error)
    {
        logUtil::writeLogMessageToConsoleAndFile("debug", typeid(OpcUAClient).name(), __LINE__, "Error code: " + error.getErrorMessage());
    }
}

std::string OpcUAClient::readService(std::string identifier)
{
    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(OpcUAClient).name(), __LINE__, "Read node: ns=2;s=" + identifier);
    try
    {
        // TODO der identifier der nodeID kann auch bytestring, guid o.ä. sein -> auslagern in eigene Methoden
    // TODO namespaceIndex variable gestalten
        char* chIdentifier = util::stringToChar(identifier);
        const UA_NodeId nodeID = UA_NODEID_STRING(2, chIdentifier);

        UA_Variant variant; /* Variants can hold scalar values and arrays of any type */
        UA_Variant_init(&variant);

        std::string value = readValue(nodeID, variant);

        logUtil::writeLogMessageToConsoleAndFile("debug", typeid(OpcUAClient).name(), __LINE__, "Value is: " + value);

        return value;
    }
    catch (OpcUAException& error)
    {
        logUtil::writeLogMessageToConsoleAndFile("debug", typeid(OpcUAClient).name(), __LINE__, "Error code: " + error.getErrorMessage());
        return "error";
    }
    
}

void OpcUAClient::cleanClient()
{
    UA_Client_delete(client);
}