#ifndef CONFIGFILEUTIL_H

#define CONFIGFILEUTIL_H

#include <string>
#include <list>

class configFileUtil
{

public:

	struct confParam {

		std::string strHostname;
		std::string strUsername;
		std::string strPassword;
		std::string strPort;
		std::string strStation;
		std::string strTag;

	};

	static confParam readConfig();

};

#endif