#include "OpcUAClient.h"
// 58
#include "logUtil.h"
#include "util.h"
#include "OpcUAException.h"
#include "open62541Util.h"
#include "serviceInput.h"

#include <open62541/client_config_default.h>

#include <stdio.h>
#include <iostream>

void OpcUAClient::createAndConnectClient(std::string url) {

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Create and conntect client to URL: " + url);
    
    client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));
    
    UA_StatusCode statusCode = UA_Client_connect(client, url.c_str());

    // TODO das ersetzen durch exceptions
    if (statusCode != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Error, could not create and connect client");
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Error code: " + statusCode);
    } else {
        logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Create and connect client successful");
    }
}
// 49
UA_BrowseResponse OpcUAClient::browseService(std::string nodeId) {

    UA_BrowseRequest request;
    UA_BrowseRequest_init(&request);
    request.requestedMaxReferencesPerNode = 0;
    request.nodesToBrowse = UA_BrowseDescription_new();
    request.nodesToBrowseSize = 1;
    request.nodesToBrowse[0].nodeId = open62541Util::createNodeId(nodeId);
    request.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;
    UA_BrowseResponse response = UA_Client_Service_browse(client, request);

    return response;
}

void OpcUAClient::writeValue(UA_NodeId nodeID, UA_Variant* variant) {

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Starte writing value");

    UA_WriteRequest writeRequest;
    UA_WriteRequest_init(&writeRequest);
    writeRequest.nodesToWrite = UA_WriteValue_new();
    writeRequest.nodesToWriteSize = 1;
    writeRequest.nodesToWrite[0].nodeId = nodeID;
    writeRequest.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
    writeRequest.nodesToWrite[0].value.hasValue = true;
    writeRequest.nodesToWrite[0].value.value.type = variant->type;
    writeRequest.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE;
    writeRequest.nodesToWrite[0].value.value.data = &variant->data;
    UA_WriteResponse writeResponse = UA_Client_Service_write(client, writeRequest);

    UA_StatusCode statusCode = writeResponse.responseHeader.serviceResult;
    
    std::string readableStatuscode = UA_StatusCode_name(statusCode);

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Statuscode: " + readableStatuscode);
    
    if (UA_StatusCode_isBad(statusCode)) {
        throw OpcUAException(statusCode);
    }
}
// 39
void OpcUAClient::writeService(serviceInput input) {

    std::string strNodeId = input.getNodeId();
    std::string datatype = input.getDatatype();
    std::string value = input.getValue();

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Writing value: " + value + " to node: " + strNodeId);

    try {

        UA_NodeId nodeId = open62541Util::createNodeId(strNodeId);

        UA_Variant* variant = UA_Variant_new();
        
        if (util::stringContainsSubstring(datatype, "Boolean")) {

            UA_Boolean booleanValue = util::stringToBool(value);

            UA_Variant_setScalar(variant, &booleanValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
        }
        else if (util::stringContainsSubstring(datatype, "float")) {
            UA_Variant_setScalar(variant, &value, &UA_TYPES[UA_TYPES_FLOAT]);
        }
        
        writeValue(nodeId, variant);
    } catch (OpcUAException& error) {
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Error code: " + error.getErrorMessage());
    }
}
// 29
UA_Variant OpcUAClient::readValue(UA_NodeId nodeID, UA_Variant variantToRead) {

    UA_StatusCode statusCode = UA_Client_readValueAttribute(client, nodeID, &variantToRead);
    
    // TODO hier wird der statuscode im schlechten fall zweimal ausgegeben
    std::string readableStatuscode = UA_StatusCode_name(statusCode);
    
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Statuscode: " + readableStatuscode);

    if (UA_StatusCode_isBad(statusCode)) {
        throw OpcUAException(statusCode);
    } else {        
        return variantToRead;
    }
}

UA_Variant OpcUAClient::readService(serviceInput argument) {

    
    UA_Variant resultVariant;
    UA_Variant_init(&resultVariant);

    std::string strNodeId = argument.getNodeId();
    std::string datatype = argument.getDatatype();
    std::string value = argument.getValue();

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Read value of node: " + strNodeId);

    try {
        UA_NodeId nodeId = open62541Util::createNodeId(strNodeId);

        resultVariant = readValue(nodeId, resultVariant);
        
    } catch (OpcUAException& error) {
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Error code: " + error.getErrorMessage());
    }

    return resultVariant;
}
// 14
std::string OpcUAClient::callMethod(std::string inputArgument, std::string strNodeId) {

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Call method, ID: " + strNodeId + "Input Argument: " + inputArgument);

    std::string result = "";

    UA_NodeId nodeId = open62541Util::createNodeId(strNodeId);

    UA_Variant input;
    UA_String argString = UA_String_fromChars(inputArgument.c_str());
    UA_Variant_setScalarCopy(&input, &argString, &UA_TYPES[UA_TYPES_STRING]);    

    size_t outputSize = 1;
    UA_Variant* output;

    UA_StatusCode statuscode = UA_Client_call(client, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER), nodeId, 1, &input, &outputSize, &output);
    std::string strStatuscode = UA_StatusCode_name(statuscode);

    // TODO ersetzen durch excpetions mit try catch
    if (statuscode == UA_STATUSCODE_GOOD) {

        UA_String* uaStringProcess = (UA_String*)output->data;

        result = open62541Util::uaStringPtrToStdString(uaStringProcess);
    } else {
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(OpcUAClient).name(), __LINE__, "Statuscode: " + strStatuscode);
    }
    return result;
}

void OpcUAClient::cleanClient() {

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(OpcUAClient).name(), __LINE__, "Clean Client");

    UA_Client_delete(client);
}