#include "util.h"
// 41
#include "logUtil.h"

#include <ctime>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <windows.h>

std::string util::getDateAndTime() {
	time_t now = time(0);
	std::tm timeStruct{};
	localtime_s(&timeStruct, &now);
	char buffer[64];
	return
	{
		buffer, std::strftime(buffer, sizeof(buffer), "%Y-%m-%d.%X", &timeStruct)
	};
}
// 38
std::vector<std::string> util::splitString(std::string stringToSplit, std::string seperator) {
	std::vector<std::string> stringVector;

	size_t pos = 0;
	std::string token;
	while ((pos = stringToSplit.find(seperator)) != std::string::npos) {

		token = stringToSplit.substr(0, pos);
		stringVector.push_back(token);
		stringToSplit.erase(0, pos + seperator.length());
	}

	stringVector.push_back(stringToSplit);

	return stringVector;
}

bool util::stringContainsSubstring(std::string string, std::string substring) {

	if (string.find(substring) != std::string::npos) {
		return true;
	}
	else {
		return false;
	}
}

std::string util::stringToUpperCaseString(std::string string) {
	std::transform(string.begin(), string.end(), string.begin(), ::toupper);
	return string;
}

std::string util::boolToString(bool value) {
	if (value) {
		return "true";
	} else {
		return "false";
	}
}

bool util::stringToBool(std::string string) {

	bool result = false;
	if (stringContainsSubstring(string, "true")) {
		result = true;
	} else if (stringContainsSubstring(string, "false")) {
		result = false;
	}
	else {
		logUtil::writeLogMessageToConsoleAndFile("debug", typeid(util).name(), __LINE__, "Error, string is not true or false");
	}

	return result;
}

void util::saveStringAsFile(std::string string, std::string filename) {

	logUtil::writeLogMessageToConsoleAndFile("debug", typeid(util).name(), __LINE__, "Save string: " + string + " in file: " + filename);

	std::ifstream checkLogFile(filename.c_str());

	std::ofstream logFile;
	logFile.open(filename, std::ios_base::app);

	logFile << string;
	logFile.close();
}
// 21
std::vector<std::string> util::getFilesInDirectory(std::string directory) {

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(util).name(), __LINE__, "Directory: " + directory);

	std::vector<std::string> files;

	for (const auto& entry : std::filesystem::directory_iterator(directory)) {

		std::string file = entry.path().string().c_str();

		std::vector<std::string> fileParts = util::splitString(file, "/");

		files.push_back(fileParts[fileParts.size() - 1]);
	}

	return files;
}

void util::createDirectory(std::string pathOfDirectory) {

	std::wstring wString = stdStringToLPCWSTR(pathOfDirectory);
	LPCWSTR lpcwstrString = wString.c_str();
	
	if (CreateDirectory(lpcwstrString, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		logUtil::writeLogMessageToConsoleAndFile("info", typeid(util).name(), __LINE__, "Success");
	} else {
		logUtil::writeLogMessageToConsoleAndFile("info", typeid(util).name(), __LINE__, "Failed");
	}
}

std::wstring util::stdStringToLPCWSTR(std::string string) {

	int length;
	int stringLength = (int)string.length() + 1;
	length = MultiByteToWideChar(CP_ACP, 0, string.c_str(), stringLength, 0, 0);
	wchar_t* buffer = new wchar_t[length];
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), stringLength, buffer, length);
	std::wstring wString(buffer);
	delete[] buffer;

	return wString;
}

std::string util::getFile(std::string filename) {

	std::string line;
	std::string text;

	std::ifstream file(filename);

	while (getline(file, line)) {

		text = text + line;
		std::cout << line << std::endl;
	}

	file.close();

	return text;
}