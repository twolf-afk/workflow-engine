#ifndef OPCUASERVER_H

#define OPCUASERVER_H

#include <open62541/server.h>

class opcuaServer {

public:

	opcuaServer();
	~opcuaServer();

	void startServer();

private:

	UA_Server* server;
	UA_Boolean running = true;

	UA_Argument createStringArgument(char argumentName[]);
	UA_MethodAttributes createMethodAttributes(char methodName[]);

	void createExecuteProcessMethod();
	void createExecuteProcessStepMethod();

};

#endif 