#include "opcuaServer.h"

#include "logUtil.h"
#include "engine.h"
#include "open62541Util.h"
#include "util.h"

#include <open62541/server_config_default.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/client_config_default.h>
#include <open62541/client.h>
#include <open62541/server.h>

#include <signal.h>
#include <stdlib.h>

UA_Argument opcuaServer::createStringArgument(char argumentName[]) {

    char locale[] = "de-GER";
    char str[] = "A String";

    UA_Argument argument;
    UA_Argument_init(&argument);
    argument.name = UA_STRING(argumentName);
    argument.description = UA_LOCALIZEDTEXT(locale, str);
    argument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    argument.valueRank = UA_VALUERANK_SCALAR;

    return argument;
}

UA_MethodAttributes opcuaServer::createMethodAttributes(char methodName[]) {

    char locale[] = "de-GER";

    UA_MethodAttributes methodeAttribute = UA_MethodAttributes_default;
    methodeAttribute.description = UA_LOCALIZEDTEXT(locale, methodName);
    methodeAttribute.displayName = UA_LOCALIZEDTEXT(locale, methodName);
    methodeAttribute.executable = true;
    methodeAttribute.userExecutable = true;

    return methodeAttribute;
}

static UA_StatusCode executeProcessStepCallBack(
    UA_Server* server, const UA_NodeId* sessionId,
    void* sessionHandle, const UA_NodeId* methodId,
    void* methodContext, const UA_NodeId* objectId,
    void* objectContext, size_t inputSize,
    const UA_Variant* input, size_t outputSize,
    UA_Variant* output) {

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "executeProcessStep was called");

    UA_String* firstInput = (UA_String*)input[0].data;

    std::string inputString = open62541Util::uaStringPtrToStdString(firstInput);
    std::vector<std::string> inputStringVector = util::splitString(inputString, ";");

    std::string processName = inputStringVector[0];
    std::string sequenceNumber = inputStringVector[1];

    Engine::getProcessFromServiceLib(processName);
    Engine::getServicesOfProcessFromServiceLib(processName);

    Engine::executeProcessStep(processName, std::atoi(sequenceNumber.c_str()));

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "executeProcessStep done");

    return UA_STATUSCODE_GOOD;
}

void opcuaServer::createExecuteProcessStepMethod() {

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "Create Execute Process Step Method");

    char inputProcessName[] = "Process Name;SequenceNumber";
    UA_Argument inputArgument = createStringArgument(inputProcessName);

    char outputText[] = "Result";
    UA_Argument outputArgument = createStringArgument(outputText);

    char methodeName[] = "Execute Process Step";
    UA_MethodAttributes methodAttributes = createMethodAttributes(methodeName);
    UA_MethodAttributes execAttr = UA_MethodAttributes_default;

    UA_Server_addMethodNode(
        server, UA_NODEID_STRING(1, methodeName),
        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, methodeName),
        execAttr, &executeProcessStepCallBack,
        1, &inputArgument, 1, &outputArgument, NULL, NULL
    );
}

static UA_StatusCode executeProcessCallback(
    UA_Server* server, const UA_NodeId* sessionId,
    void* sessionHandle, const UA_NodeId* methodId,
    void* methodContext, const UA_NodeId* objectId,
    void* objectContext, size_t inputSize, 
    const UA_Variant* input, size_t outputSize,
    UA_Variant* output) {

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "executeProcessCallback was called");

    UA_String* Input = (UA_String*)input->data;
    std::string processName = open62541Util::uaStringPtrToStdString(Input);

    Engine::getProcessFromServiceLib(processName);
    Engine::getServicesOfProcessFromServiceLib(processName);

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "Input Argument: " + processName);

    Engine::executeProcess(processName);

    return UA_STATUSCODE_GOOD;
}

void opcuaServer::createExecuteProcessMethod() {

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "Create Exeucte Process Method");

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

    UA_Server_addMethodNode(
        server, UA_NODEID_STRING(1, executeProcess),
        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, executeProcess),
        execAttr, &executeProcessCallback,
        1, &inputArgument, 1, &outputArgument, NULL, NULL
    );
}

opcuaServer::opcuaServer() {

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "Create OPC UA Server");

    server = UA_Server_new();
    UA_ServerConfig* config = UA_Server_getConfig(server);

    UA_ServerConfig_setMinimal(config, 10001, NULL);
    UA_String_clear(&config->applicationDescription.applicationUri);
    config->applicationDescription.applicationUri = UA_String_fromChars("urn:workflow-engine");

    createExecuteProcessMethod();
    createExecuteProcessStepMethod();
}

void opcuaServer::startServer() {

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "Start OPC UA Server Instance");

    UA_StatusCode status = UA_Server_run(server, &running);
}

opcuaServer::~opcuaServer() {

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "Delete OPC UA Server Instance");

    UA_Server_delete(server);
}