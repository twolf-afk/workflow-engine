
#include "orchestrierungMinimalBeispiel.h"

#include "logUtil.h"
#include "engine.h"
#include "service.h"


int main()
{

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(orchestrierungMinimalBeispiel).name(), __LINE__, "Start Orchestration");


	Engine::executeService("test.wsdl");

}
