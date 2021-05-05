#include "serviceTemplate.h"

#include "configFileUtil.h"

#include <fstream>

serviceTemplate::serviceTemplate() {

	documentation = "";
	url = "";

	portType = "";

	inputMessage = "";
}

// TODO nicht "AsXml" das ergebnis ist eine wsdl datei -> aendern
void serviceTemplate::createServiceAsXml(string filename) {

	configFileUtil::confParam parameter = configFileUtil::readConfig();

	filename = parameter.pathToServices + filename;

	std::ofstream wsdlFile(filename);
	wsdlFile << "<?xml version=\"1.0\"?>" << endl;

	wsdlFile << "<definitions name=\"StockQuote\" "
		<< "targetNamespace=\"http://example.com/stockquote.wsdl\" xmlns:tns =\"http://example.com/stockquote.wsdl\" "
		<< "xmlns:xsd1=\"http://example.com/stockquote.xsd\" "
		<< "xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\" "
 		<< "xmlns=\"http://schemas.xmlsoap.org/wsdl/ \"> " << endl;

	wsdlFile << "<service name = \"" << portType << "\"> " << endl
		<< "<documentation>" << documentation << "</documentation>" << endl
		<< "<port name = \"" << portType << "\" binding = \"tns:Trigger\">" << endl
		<< "<soap:address location = \"" << url << "\"/>" << endl
		<< "</port>"  << endl
		<< "</service>" << endl;

	wsdlFile << "<portType name =\"" << portType << "\">" << endl
		<< "<operation name =\"" << inputMessage << "\">" << endl
		<< "<input message =\"" << inputMessage << "\"/>" << endl
		<< "<output message =\"processDone\"/>" << endl
		<< "</operation>" << endl
		<< "</portType>" << endl;
		
	wsdlFile << "<message name = \"" << inputMessage << "\">" << endl
		<< "<part name = \"body\" element=\"xsd1:Switch\"/>" << endl
		<< "</message>" << endl;

	wsdlFile << "<message name = \"processDone\">" << endl
		<< "<part name = \"body\" element=\"xsd1:Status\"/>" << endl
		<< "</message>" << endl;

	wsdlFile << "<types>" << endl
		<< "<schema targetNamespace = \"http://example.com/stockquote.xsd\" xmlns = \"http://www.w3.org/2000/10/XMLSchema\">" << endl
		<< "<element name = \" Switch \">" << endl
		<< "<complexType>" << endl
		<< "<all>" << endl;
	map<int, std::string>::iterator iteratorInputArguments;

	for (iteratorInputArguments = inputArguments.begin(); iteratorInputArguments != inputArguments.end(); iteratorInputArguments++)	{
		wsdlFile << iteratorInputArguments->second << endl;
	}

	wsdlFile << "</all>" << endl
		<< "</complexType>" << endl
		<< "</element>" << endl
		<< "<element name =\"Status\">" << endl
		<< "<complexType>" << endl
		<< "<all>" << endl;

	map<int, std::string>::iterator iteratorOutputArguments;
	for (iteratorOutputArguments = outputArguments.begin(); iteratorOutputArguments != outputArguments.end(); iteratorOutputArguments++) {
		wsdlFile << iteratorOutputArguments->second << endl;
	}

	wsdlFile << "</all>" << endl
		<< "</complexType>" << endl
		<< "</element>" << endl
		<< "</schema>" << endl
		<< "</types>" << endl;
		
	wsdlFile << "</definitions>";
	wsdlFile.close();
}

void serviceTemplate::setDocumentation(string Documentation) {
	documentation = Documentation;
}

string serviceTemplate::getDocumentation() {
	return documentation;
}

void serviceTemplate::setUrl(string Url) {
	url = Url;
}

string serviceTemplate::getUrl() {
	return url;
}

void serviceTemplate::setPortType(string PortType) {
	portType = PortType;
}

string serviceTemplate::getPortType() {
	return portType;
}

void serviceTemplate::setInputMessage(string InputMessage) {
	inputMessage = InputMessage;
}

string serviceTemplate::getInputMessage() {
	return inputMessage;
}

void serviceTemplate::createInputArgument(int index, std::string name, std::string datatype) {
	std::string argument = "<element name =\"" + name + "\" type =\"" + datatype + "\" value =\"\"/>";
	inputArguments.insert(std::make_pair(index, argument));
}

void serviceTemplate::createOutputArgument(int index, std::string name, std::string datatype) {
	std::string argument = "<element name =\"" + name + "\" type =\"" + datatype + "\" value =\"\"/>";
	outputArguments.insert(std::make_pair(index, argument));
}