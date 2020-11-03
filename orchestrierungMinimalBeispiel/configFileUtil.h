#ifndef CONFIGFILEUTIL_H

#define CONFIGFILEUTIL_H

#include <string>
#include <list>

class configFileUtil
{

public:

	struct confParam {

		std::string pathToServices;
		std::string pathToProcesses;

	};

	static confParam readConfig();

};

#endif