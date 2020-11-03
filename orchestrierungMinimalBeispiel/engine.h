#ifndef ENGINE_H

#define ENGINE_H

#include <string>

class Engine 
{

public:

	static void executeService(std::string serviceName);

	static void executeProcess(std::string processName);

};

#endif
