#include "configFileUtil.h"

#include <fstream>
#include <string>

configFileUtil::confParam configFileUtil::readConfig()
{
	confParam parameter;

	std::ifstream file("../dbconfiguration.cfg");
	std::string strLine;

	while (std::getline(file, strLine))
	{

		if (strLine.find("services") != std::string::npos)
		{
			parameter.pathToServices = strLine.substr(strLine.find("=") + 2, strLine.length());
		}

		if (strLine.find("processes") != std::string::npos)
		{
			parameter.pathToProcesses = strLine.substr(strLine.find("=") + 2, strLine.length());
		}

	}

	return parameter;

}