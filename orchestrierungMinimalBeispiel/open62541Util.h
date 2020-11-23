#ifndef OPEN62541UTIL_H

#define OPEN62541UTIL_H

#include "open62541/types.h"

#include <string>

class open62541Util
{

public:

	static std::string uaStringPtrToStdString(UA_String* input);

};

#endif