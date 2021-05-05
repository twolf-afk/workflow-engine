
#include "workflowEngine.h"

#include "logUtil.h"
#include "opcuaServer.h"
#include "init.h"

int main() {

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(workflowEngine).name(), __LINE__, "Start Workflow Engine");
	
	init::resetDirectories();
	init::resetDirectories();

	init::createServiceDescription();

	opcuaServer server = opcuaServer();
	server.startServer();

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(workflowEngine).name(), __LINE__, "Stop Workflow Engine");

	server.~opcuaServer();
}