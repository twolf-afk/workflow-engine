#include "init.h"

#include "configFileUtil.h"
#include "createServices.h"
#include "logUtil.h"
#include "util.h"
#include "serviceLibCommunication.h"

#include <filesystem>

void init::createServiceDescription() {

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(init).name(), __LINE__, "Create Service Descritions");

	configFileUtil::confParam parameter = configFileUtil::readConfig();
	std::vector<std::string> urls = util::splitString(parameter.serverToBrowse, ";");

	size_t numberOfUrls = urls.size();
	
	for (size_t i = 0; i < numberOfUrls; i++) {

		std::string url = urls.at(i);

		createServices::browseServerAndCreateServices(url);
	}

	serviceLibCommunication::saveServicesInServiceLib();

	logUtil::writeLogMessageToConsoleAndFile("info", typeid(init).name(), __LINE__, "Create Service Descritions done");
}

void deleteFiles(const std::string& dir_path) {

	for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
		std::filesystem::remove_all(entry.path());
	}
}

void init::resetDirectories() {

	configFileUtil::confParam parameter = configFileUtil::readConfig();

	const std::string& pathToProcesses = parameter.pathToProcesses;
	const std::string& pathToServices = parameter.pathToServices;

	deleteFiles(pathToProcesses);
	deleteFiles(pathToServices);
}