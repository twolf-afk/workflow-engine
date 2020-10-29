
#include "orchestrierungMinimalBeispiel.h"
#include "logUtil.h"
#include "engine.h"

#include <typeinfo>

int main()
{

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(orchestrierungMinimalBeispiel).name(), __LINE__, "Start Orchestration");

	Engine::executeService("testService.wsdl");

}
