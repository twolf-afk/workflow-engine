#ifndef OPEN62541UTIL_H

#define OPEN62541UTIL_H

#include "open62541/types.h"

#include <string>

class open62541Util {

public:

	static std::string uaDataTypeToString(UA_Variant variant);

	static bool convertUaVariantToBool(UA_Variant variant);

	static std::string uaStringPtrToStdString(UA_String* input);

	static UA_NodeId createNodeId(std::string strNodeId);

	static std::string uaNodeIdToStdString(UA_NodeId nodeId);

	static std::string uaStringToStdString(UA_String uaString);

};

#endif