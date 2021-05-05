#include "open62541Util.h"
// 33
#include "util.h"
#include "logUtil.h"

#include "open62541\types_generated_handling.h"

#include <vector>

std::string open62541Util::uaDataTypeToString(UA_Variant variant) {
    std::string datatype = variant.type->typeName;

    return datatype;
}

bool open62541Util::convertUaVariantToBool(UA_Variant variant) {
    UA_Boolean returnValue = (UA_Boolean)variant.data;
    return returnValue;
}
// 27
std::string open62541Util::uaStringPtrToStdString(UA_String* input) {

    UA_String inputString;
    UA_String_init(&inputString);

    if (input->length > 0) {
        inputString.data = (UA_Byte*)UA_realloc(inputString.data, inputString.length + input->length);
        memcpy(&inputString.data[inputString.length], input->data, input->length);
        inputString.length += input->length;
    }

    char* inputChar = (char*)UA_malloc(sizeof(char) * inputString.length + 1);
    memcpy(inputChar, inputString.data, inputString.length);
    inputChar[inputString.length] = '\0';

    std::string result(inputChar);

	return result;
}

UA_NodeId open62541Util::createNodeId(std::string strNodeId) {
    std::vector<std::string> idParts = util::splitString(strNodeId, ";");

    std::vector<std::string> nsParts = util::splitString(idParts[0], "=");

    UA_Int16 ns = std::atoi(nsParts[1].c_str());

    std::vector<std::string> identifierParts = util::splitString(idParts[1], "=");

    std::string identifiertype = identifierParts[0];

    std::string identifier = identifierParts[1];

    UA_NodeId nodeId = UA_NODEID_STRING_ALLOC(0, "");
    if (!identifiertype.compare("s")) {
        nodeId = UA_NODEID_STRING_ALLOC(ns, identifier.c_str());
    } else if (!identifiertype.compare("i")) {
        nodeId = UA_NODEID_NUMERIC(ns, std::atoi(identifier.c_str()));
    } else if (!identifiertype.compare("guid")) {

    } else if (!identifiertype.compare("opague")) {

    } else {
        logUtil::writeLogMessageToConsoleAndFile("error", typeid(open62541Util).name(), __LINE__, "Unkown identifiertype: " + identifiertype);
    }
    return nodeId;
}
// 12
std::string open62541Util::uaNodeIdToStdString(UA_NodeId nodeId) {

    int ns = nodeId.namespaceIndex;
    std::string stdNodeId = "ns=" + std::to_string(ns);

    if (nodeId.identifierType == UA_NODEIDTYPE_STRING) {
        std::string identifier = open62541Util::uaStringToStdString(nodeId.identifier.string);

        stdNodeId = stdNodeId + ";s=" + identifier;

    } else if (nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
        int identifier = nodeId.identifier.numeric;

        stdNodeId = stdNodeId + ";i=" + std::to_string(identifier);
    } else if (nodeId.identifierType == UA_NODEIDTYPE_GUID) {

    } else if (nodeId.identifierType == UA_NODEIDTYPE_BYTESTRING) {

    }

    return stdNodeId;
}

std::string open62541Util::uaStringToStdString(UA_String uaString) {

    std::string stdString = std::string((const char*)uaString.data);
    stdString = stdString.substr(0, uaString.length);

    return stdString;
}