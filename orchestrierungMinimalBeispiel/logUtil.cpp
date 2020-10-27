#include "logUtil.h"

#include "util.h"

#include <fstream>
#include <iostream>

void logUtil::writeLogMessageToConsoleAndFile(std::string logLevel, std::string Class, std::int32_t line, std::string message)
{

	std::string strLogMes = createLogMessage(logLevel, Class, line, message);

	std::cout << strLogMes;


	std::string logFileName = "logFile.txt";
	std::ifstream checkLogFile(logFileName.c_str());

	if (checkLogFile.good())
	{
		std::ofstream logFile;
		logFile.open(logFileName, std::ios_base::app);

		logFile << strLogMes;
		logFile.close();
	}
}

std::string logUtil::createLogMessage(std::string logLevel, std::string Class, std::int32_t line, std::string message)
{

	return "["
		+ util::getDateAndTime()
		+ "]["
		+ util::stringToUpperCaseString(logLevel) + "]"
		+ "[" + Class + "]"
		+ "[" + "Line: "
		+ std::to_string(line) + "] "
		+ message
		+ "\r\n";
}