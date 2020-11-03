
#include "orchestrierung.h"

#include "logUtil.h"
#include "engine.h"
#include "configFileUtil.h"

int main()
{

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(orchestrierung).name(), __LINE__, "Start Orchestration");

	configFileUtil::confParam parameter = configFileUtil::readConfig();

	Engine::executeProcess(parameter.pathToProcesses + "indexfile.xml");

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(orchestrierung).name(), __LINE__, "Stop Orchestration");
}
