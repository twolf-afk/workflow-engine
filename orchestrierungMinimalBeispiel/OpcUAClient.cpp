#include "OpcUAClient.h"

#include "logUtil.h"
#include "util.h"
#include "OpcUAException.h"
#include "open62541Util.h"

#include <open62541/client_config_default.h>

#include <stdio.h>
#include <iostream>

void OpcUAClient::createAndConnectClient(std::string url)
{
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Create and conntect client to URL: " + url);

    client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));
    
    UA_StatusCode statusCode = UA_Client_connect(client, url.c_str());
    
    // TODO das ersetzen durch exceptions
    if (statusCode != UA_STATUSCODE_GOOD)
    {
        UA_Client_delete(client);
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Error, could not create and connect client");
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Error code: " + statusCode);
    }
    else
    {
        logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Create and connect client successful");
    }
}

UA_NodeId createNodeId(std::string strNodeID)
{
    std::vector<std::string> idParts = util::splitString(strNodeID, ";");

    std::vector<std::string> nsParts = util::splitString(idParts[0], "=");

    UA_Int16 ns = std::atoi(nsParts[1].c_str());

    std::vector<std::string> identifierParts = util::splitString(idParts[1], "=");

    std::string identifiertype = identifierParts[0];

    std::string identifier = identifierParts[1];

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "NS: " + nsParts[1] + ", identifiertype: " + identifiertype + ", identifier: " + identifier);

    UA_NodeId nodeId = UA_NODEID_STRING_ALLOC(0, "");
    if (identifiertype == "s")
    {
        nodeId = UA_NODEID_STRING_ALLOC(ns, identifier.c_str());
    }
    else if (identifier == "i")
    {
        nodeId = UA_NODEID_NUMERIC(ns, std::atoi(identifier.c_str()));
    }
    else if (identifier == "guid")
    {
        // nodeId = UA_NODEID_GUID(ns, );
    }
    else if (identifier == "opaque")
    {

    }
    else
    {
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Unkown identifiertype: " + identifiertype);
    }
    return nodeId;
}

void OpcUAClient::writeValue(UA_NodeId nodeID, UA_Variant* variant)
{
    UA_StatusCode statusCode = UA_Client_writeValueAttribute(client, nodeID, variant);

    std::string readableStatuscode = UA_StatusCode_name(statusCode);

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Statuscode: " + readableStatuscode);
    
    if (UA_StatusCode_isBad(statusCode))
    {
        throw OpcUAException(statusCode);
    }

}

void OpcUAClient::writeService(std::queue<serviceInput> inputs)
{

    size_t inputSize = inputs.size();

    for (size_t i = 0; i < inputSize; i++)
    {
        serviceInput input = inputs.front();
        inputs.pop();

        std::string strNodeId = input.getNodeId();
        std::string datatype = input.getDatatype();
        std::string value = input.getValue();

        logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Writing value: " + value + " to node: " + strNodeId);

        try
        {

            UA_NodeId nodeId = createNodeId(strNodeId);

            UA_Variant* variant = UA_Variant_new();

            if (datatype == "bool")
            {
                UA_Variant_setScalar(variant, &value, &UA_TYPES[UA_TYPES_BOOLEAN]);
            }
            else if (datatype == "float")
            {
                UA_Variant_setScalar(variant, &value, &UA_TYPES[UA_TYPES_FLOAT]);
            }

            writeValue(nodeId, variant);

        }
        catch (OpcUAException& error)
        {
            logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Error code: " + error.getErrorMessage());
        }

    }
}

bool OpcUAClient::readValue(UA_NodeId nodeID, UA_Variant variantToRead)
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
        UA_Boolean uaResult = *(UA_Boolean*) variantToRead.data;
        
        return uaResult;
    }
}

bool OpcUAClient::readService(std::queue<serviceInput> outputs)
{
    int inputSize = outputs.size();

    for (int i = 0; i < inputSize; i++)
    {
        serviceInput output = outputs.front();
        outputs.pop();

        std::string strNodeId = output.getNodeId();
        std::string datatype = output.getDatatype();
        std::string value = output.getValue();

        logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Writing value: " + value + " to node: " + strNodeId);

        try
        {
            UA_NodeId nodeId = createNodeId(strNodeId);

            UA_Variant resultVariant;
            UA_Variant_init(&resultVariant);

            bool result = readValue(nodeId, resultVariant);

            logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Value is: " + util::boolToString(result));

            return result;
        }
        catch (OpcUAException& error)
        {
            logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Error code: " + error.getErrorMessage());
            return false;
        }
    }
}

std::string OpcUAClient::callMethod(std::string inputArgument, std::string strNodeId)
{
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Call method, ID: " + strNodeId + "Input Argument: " + inputArgument);

    std::string result = "";

    UA_NodeId nodeId = createNodeId(strNodeId);

    UA_Variant input;
    UA_String argString = UA_String_fromChars(inputArgument.c_str());
    UA_Variant_init(&input);
    UA_Variant_setScalarCopy(&input, &argString, &UA_TYPES[UA_TYPES_STRING]);

    size_t outputSize = 1;
    UA_Variant* output;

    UA_StatusCode statuscode = UA_Client_call(client, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER), nodeId, 1, &input, &outputSize, &output);
    std::string strStatuscode = UA_StatusCode_name(statuscode);

    // TODO ersetzen durch excpetions mit try catch
    if (statuscode == UA_STATUSCODE_GOOD)
    {

        UA_String* uaStringProcess = (UA_String*)output->data;

        result = open62541Util::uaStringPtrToStdString(uaStringProcess);
    }
    else
    {
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Statuscode: " + strStatuscode);
    }
    return result;
}

void OpcUAClient::cleanClient()
{
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Clean Client");

    UA_Client_delete(client);
}