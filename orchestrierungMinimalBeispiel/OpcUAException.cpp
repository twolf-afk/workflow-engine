#include "OpcUAException.h"

OpcUAException::OpcUAException(UA_StatusCode statusCode)
{

	errorMessage = UA_StatusCode_name(statusCode);

}

std::string OpcUAException::getErrorMessage()
{
	return errorMessage;
}
