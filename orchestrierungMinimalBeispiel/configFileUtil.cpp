#include "configFileUtil.h"

#include <fstream>
#include <string>

configFileUtil::confParam configFileUtil::readConfig()
{
	confParam parameter;

	std::ifstream file("dbconfiguration.cfg");
	std::string strLine;

	while (std::getline(file, strLine))
	{

		if (strLine.find("hostname") != std::string::npos)
		{
			parameter.strHostname = strLine.substr(strLine.find("=") + 2, strLine.length());
		}

		if (strLine.find("username") != std::string::npos)
		{
			parameter.strUsername = strLine.substr(strLine.find("=") + 2, strLine.length());
		}

		if (strLine.find("password") != std::string::npos)
		{
			parameter.strPassword = strLine.substr(strLine.find("=") + 2, strLine.length());
		}

		if (strLine.find("port") != std::string::npos)
		{
			parameter.strPort = strLine.substr(strLine.find("=") + 2, strLine.length());
		}

		if (strLine.find("station") != std::string::npos)
		{
			parameter.strStation = strLine.substr(strLine.find("=") + 2, strLine.length());
		}
		if (strLine.find("barcodeTag") != std::string::npos)
		{
			parameter.strTag = strLine.substr(strLine.find("=") + 2, strLine.length());
		}

	}

	return parameter;

}