#ifndef SERVICE_H

#define SERVICE_H

#include <string>
#include <map>

using namespace std;

class serviceTemplate {

private:

	string documentation;
	string url;
	
	string portType;

	string inputMessage;
	std::map<int, std::string> inputArguments;

	std::map<int, std::string> outputArguments;

public:

	serviceTemplate();

	void createServiceAsXml(string filename);

	void setDocumentation(string Documentation);
	string getDocumentation();

	void setUrl(string Url);
	string getUrl();

	void setPortType(string PortType);
	string getPortType();

	void setInputMessage(string InputMessage);
	string getInputMessage();

	void createInputArgument(int index, std::string name, std::string datatype);
	void createOutputArgument(int index, std::string name, std::string datatype);

};

#endif