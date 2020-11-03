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
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Error, could not create and connect client");
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Error code: " + statusCode);
        return false;
    }
    else
    {
        logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Create and connect client successful");
        return true;
    }
}

void OpcUAClient::writeValue(UA_NodeId nodeID, UA_Variant variant)
{
    UA_StatusCode statusCode = UA_Client_writeValueAttribute(client, nodeID, &variant);

    std::string readableStatuscode = UA_StatusCode_name(statusCode);

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Statuscode: " + readableStatuscode);
    
    if (UA_StatusCode_isBad(statusCode))
    {
        throw OpcUAException(statusCode);
    }

}

void OpcUAClient::writeService(std::string value, std::string identifier)
{
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Writing value: " + value + " to node: ns=1;s=" + identifier);
    
    try
    {
        // TODO der identifier der nodeID kann auch bytestring, guid o.ä. sein -> auslagern in eigene Methoden
        // TODO namespaceIndex variable gestalten
        UA_NodeId nodeID = UA_NODEID_STRING_ALLOC(1, identifier.c_str());

        // TODO neben in16 gibt es noch viele weitere datentypen, hierzu jeweils eine methode erstelleb, ggf. mit vererbung, abstraktion, interfaces etc.
        UA_String uaString = UA_String_fromChars(value.c_str());
        UA_Variant* variant = UA_Variant_new();
        UA_Variant_setScalar(variant, &uaString, &UA_TYPES[UA_TYPES_STRING]);

        writeValue(nodeID, *variant);
        
    }
    catch (OpcUAException& error)
    {
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Error code: " + error.getErrorMessage());
    }
}

std::string OpcUAClient::readValue(UA_NodeId nodeID, UA_Variant variantToRead)
{
    UA_StatusCode statusCode = UA_Client_readValueAttribute(client, nodeID, &variantToRead);

    // TODO hier wird der statuscode im schlechten fall zweimal ausgegeben
    std::string readableStatuscode = UA_StatusCode_name(statusCode);

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Statuscode: " + readableStatuscode);

    if (UA_StatusCode_isBad(statusCode))
    {
        throw OpcUAException(statusCode);
    }
    else
    {
        UA_String uaResult = *(UA_String*) variantToRead.data;

        // TODO auslagern in util
        char* convert = (char*)UA_malloc(sizeof(char) * uaResult.length + 1);
        memcpy(convert, uaResult.data, uaResult.length);
        convert[uaResult.length] = '\0';

        // TODO result umbenennen
        std::string result = convert;

        return result;
    }
}

std::string OpcUAClient::readService(std::string identifier)
{
    logUtil::writeLogMessageToConsoleAndFile("debug", typeid(OpcUAClient).name(), __LINE__, "Read node: ns=1;s=" + identifier);
    try
    {
        // TODO der identifier der nodeID kann auch bytestring, guid o.ä. sein -> auslagern in eigene Methoden
        // TODO namespaceIndex variable gestalten
        char* chIdentifier = util::stringToChar(identifier);
        const UA_NodeId nodeID = UA_NODEID_STRING(1, chIdentifier);

        UA_Variant resultVariant; /* Variants can hold scalar values and arrays of any type */
        UA_Variant_init(&resultVariant);

        std::string value = readValue(nodeID, resultVariant);

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