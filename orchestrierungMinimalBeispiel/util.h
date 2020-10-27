#ifndef UTIL_H

#define UTIL_H

#include <string>
#include <vector>

class util
{

public:

	static std::string getDateAndTime();

	static std::vector<std::string> splitString(std::string stringToSplit, std::string seperator);

	static std::string stringToUpperCaseString(std::string string);

};

#endif