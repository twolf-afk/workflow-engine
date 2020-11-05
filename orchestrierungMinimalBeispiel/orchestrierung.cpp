
#include "orchestrierung.h"

#include "logUtil.h"
#include "opcuaServer.h"

int main()
{

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(orchestrierung).name(), __LINE__, "Start Orchestration");

	opcuaServer server = opcuaServer();
	server.startServer();


	

	

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(orchestrierung).name(), __LINE__, "Stop Orchestration");

	server.~opcuaServer();
}
