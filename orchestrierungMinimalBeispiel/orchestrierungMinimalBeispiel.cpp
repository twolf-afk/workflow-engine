
#include "orchestrierungMinimalBeispiel.h"
#include "logUtil.h"
#include "wsdlHandler.h"

#include <typeinfo>

int main()
{

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(orchestrierungMinimalBeispiel).name(), __LINE__, "Start Orchestration");

	wsdlHandler::readWsdlFile("testService.wsdl");

}
