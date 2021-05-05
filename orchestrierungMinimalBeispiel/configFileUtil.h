#ifndef CONFIGFILEUTIL_H

#define CONFIGFILEUTIL_H

#include <string>
#include <list>

class configFileUtil {

public:

	struct confParam {

		std::string pathToServices;
		std::string pathToProcesses;

		std::string serviceLibURL;

		std::string serverToBrowse;

	};

	static confParam readConfig();

};

#endif