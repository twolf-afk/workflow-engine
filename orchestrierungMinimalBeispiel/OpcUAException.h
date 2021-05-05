#ifndef OPCUAEXCEPTION_H

#define OPCUAEXCEPTION_H

#include <open62541\types.h>

#include <exception>
#include <string>

class OpcUAException : public std::exception {

private:

	std::string errorMessage;

public:

	OpcUAException(UA_StatusCode statusCode);

	virtual std::string getErrorMessage();

};

#endif