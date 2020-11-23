#include "opcuaServer.h"

#include "logUtil.h"
#include "engine.h"
#include "configFileUtil.h"
#include "open62541Util.h"
#include "OpcUAClient.h"
#include "util.h"

#include <open62541/server_config_default.h>
#include <open62541/plugin/log_stdout.h>

#include <iostream>

static UA_StatusCode executeProcessCallback
(
    UA_Server* server, const UA_NodeId* sessionId,
    void* sessionHandle, const UA_NodeId* methodId,
    void* methodContext, const UA_NodeId* objectId,
    void* objectContext, size_t inputSize, 
    const UA_Variant* input, size_t outputSize,
    UA_Variant* output
)
{
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "executeProcessCallback was called");

    UA_String* firstInput = (UA_String*)input->data;
    std::string processName = open62541Util::uaStringPtrToStdString(firstInput);

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "Input Argument: " + processName);

    // OpcUAClient client;
    // client.createAndConnectClient("opc.tcp://DESKTOP-0AULV4D:2/");

    // std::string process = client.callMethod(processName, "ns=1;s=Get Process");

    // client.cleanClient();

    configFileUtil::confParam parameter = configFileUtil::readConfig();
    // util::saveStringAsFile(process, parameter.pathToProcesses + processName);

    Engine::executeProcess(parameter.pathToProcesses + processName);

    // TODO Fehlerbehandlung einbauen und nicht immer statuscod_good zurueckgeben
    return UA_STATUSCODE_GOOD;

}

void opcuaServer::createAdressSpace()
{
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "Create Addressspace");

    char locale[] = "de-GER";
    char str[] = "A String";

    UA_Argument inputArgument;
    UA_Argument_init(&inputArgument);
    char inputText[] = "Name of Indexfile; Format: \"filename.xml\"";
    inputArgument.name = UA_STRING(inputText);
    inputArgument.description = UA_LOCALIZEDTEXT(locale, str);
    inputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    inputArgument.valueRank = UA_VALUERANK_SCALAR;


    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    char outputText[] = "Result";
    outputArgument.name = UA_STRING(outputText);
    outputArgument.description = UA_LOCALIZEDTEXT(locale, str);
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;


    UA_MethodAttributes execAttr = UA_MethodAttributes_default;
    char executeProcess[] = "Execute Process";
    execAttr.description = UA_LOCALIZEDTEXT(locale, executeProcess);
    execAttr.displayName = UA_LOCALIZEDTEXT(locale, executeProcess);
    execAttr.executable = true;
    execAttr.userExecutable = true;

    UA_Server_addMethodNode
    (
        server, UA_NODEID_NUMERIC(1, 62541),
        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, executeProcess),
        execAttr, &executeProcessCallback,
        1, &inputArgument, 1, &outputArgument, NULL, NULL
    );

}

opcuaServer::opcuaServer()
{
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "Create OPC UA Server");

    server = UA_Server_new();
    UA_ServerConfig* config = UA_Server_getConfig(server);
    UA_ServerConfig_setDefault(config);

    UA_ServerConfig_setMinimal(config, 1, NULL);
    UA_String_clear(&config->applicationDescription.applicationUri);
    config->applicationDescription.applicationUri = UA_String_fromChars("urn:workflow-engine");


    createAdressSpace();

}

void opcuaServer::startServer()
{
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "Start OPC UA Server Instance");

    UA_StatusCode status = UA_Server_run(server, &running);
}

opcuaServer::~opcuaServer()
{
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "Delete OPC UA Server Instance");

    UA_Server_delete(server);
}