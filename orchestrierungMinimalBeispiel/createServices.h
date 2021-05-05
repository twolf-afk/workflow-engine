#ifndef CREATESERVICES_H

#define CREATESERVICES_H

#include <string>

class createServices {

public:

	static void browseServerAndCreateServices(std::string url);
	static void browseServerAndCreateServicesRecursion(std::string nodeId);

};

#endif