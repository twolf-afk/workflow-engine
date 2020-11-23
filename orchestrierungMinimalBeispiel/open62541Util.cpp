#include "open62541Util.h"

#include "open62541\types_generated_handling.h"

std::string open62541Util::uaStringPtrToStdString(UA_String* input)
{

    UA_String inputString;
    UA_String_init(&inputString);

    if (input->length > 0)
    {
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
