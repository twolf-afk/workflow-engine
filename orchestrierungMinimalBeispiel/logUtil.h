#ifndef LOGUTIL_H

#define LOGUTIL_H

#include <string>
#include <typeinfo>

class logUtil {

public:

	static void writeLogMessageToConsoleAndFile(std::string logLevel, std::string Class, std::int32_t line, std::string message);

private:

	static std::string createLogMessage(std::string logLevel, std::string Class, std::int32_t line, std::string message);

};

#endif