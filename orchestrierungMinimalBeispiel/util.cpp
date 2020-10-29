#include "util.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include <ctime>

std::string util::getDateAndTime()
{
	time_t now = time(0);
	std::tm timeStruct{};
	localtime_s(&timeStruct, &now);
	char buffer[64];
	return
	{
		buffer, std::strftime(buffer, sizeof(buffer), "%Y-%m-%d.%X", &timeStruct)
	};
}

std::vector<std::string> util::splitString(std::string stringToSplit, std::string seperator)
{
	std::vector<std::string> stringVector;
	boost::split(stringVector, stringToSplit, boost::is_any_of(seperator));
	return stringVector;
}

std::string util::stringToUpperCaseString(std::string string)
{
	boost::to_upper(string);
	return string;
}

std::string util::boolToString(bool value)
{
	if (value)
	{
		return "true";
	}
	else
	{
		return "false";
	}
}

char* util::stringToChar(std::string string)
{

	rsize_t lengthOfChar = string.length() + 1;
	char* Char = new char[lengthOfChar];
	strcpy_s(Char, lengthOfChar, string.c_str());
	return Char;
}