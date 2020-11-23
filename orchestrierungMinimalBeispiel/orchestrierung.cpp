
#include "orchestrierung.h"

#include "logUtil.h"
#include "opcuaServer.h"
#include "wsdlParser.h"

int main()
{

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(orchestrierung).name(), __LINE__, "Start Orchestration");

	wsdlParser serviceWsdl = wsdlParser();
	serviceWsdl.initXmlParserGetDocumentGetRootElement("../services/PneumaticCyclinder.wsdl");

	serviceWsdl.getInput();


	opcuaServer server = opcuaServer();
	server.startServer();

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(orchestrierung).name(), __LINE__, "Stop Orchestration");

	server.~opcuaServer();
}
