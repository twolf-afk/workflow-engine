#include "opcuaServer.h"

#include "logUtil.h"
#include "engine.h"
#include "configFileUtil.h"

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
    UA_String* ptrInput = (UA_String*)input->data;
    UA_String inputValue;
    UA_String_init(&inputValue);

    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "executeProcessCallback was called");

    if (ptrInput->length > 0)
    {
        inputValue.data = (UA_Byte*)UA_realloc(inputValue.data, inputValue.length + ptrInput->length);
        memcpy(&inputValue.data[inputValue.length], ptrInput->data, ptrInput->length);
        inputValue.length += ptrInput->length;
    }

    char* chValue = (char*)UA_malloc(sizeof(char) * inputValue.length + 1);
    memcpy(chValue, inputValue.data, inputValue.length);
    chValue[inputValue.length] = '\0';

    std::string value(chValue);
    logUtil::writeLogMessageToConsoleAndFile("info", typeid(opcuaServer).name(), __LINE__, "Execute Process: " + value);

    configFileUtil::confParam parameter = configFileUtil::readConfig();
    Engine::executeProcess(parameter.pathToProcesses + value);

    UA_Variant_setScalarCopy(output, &inputValue, &UA_TYPES[UA_TYPES_STRING]);
    UA_String_clear(&inputValue);
    

    return UA_STATUSCODE_GOOD;

}

void opcuaServer::createAdressSpace()
{

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

    server = UA_Server_new();
	UA_ServerConfig_setDefault(UA_Server_getConfig(server));
    createAdressSpace();

}

void opcuaServer::startServer()
{
    UA_StatusCode status = UA_Server_run(server, &running);
}

opcuaServer::~opcuaServer()
{
    UA_Server_delete(server);
}