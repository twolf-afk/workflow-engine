#ifndef ENGINE_H

#define ENGINE_H

#include <string>

class Engine {

private:

	static void executeService(std::string processName, std::string serviceName);

public:

	static void getProcessFromServiceLib(std::string processName);
	static void getServicesOfProcessFromServiceLib(std::string processName);

	static void executeProcessStep(std::string processName, int sequenceNumber);
	static void executeProcess(std::string processName);

};

#endif