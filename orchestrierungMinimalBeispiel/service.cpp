#include "service.h"

#include "configFileUtil.h"

#include <fstream>

service::service()
{
	name = "";
	url = "";

	portType = "";
	operation = "";

	inputMessage = "";
	inputName = "";
	inputElementName = "";
	inputDataType = "";
	inputValue = "";

	outputMessage = "";
	outputName = "";
	outputElementName = "";
	outputDataType = "";
	outputValue = "";
}

// TODO nicht "AsXml" das ergebnis ist eine wsdl datei -> aendern
void service::createServiceAsXml(string filename)
{

	configFileUtil::confParam parameter = configFileUtil::readConfig();

	filename = parameter.pathToServices + filename;

	std::ofstream wsdlFile(filename);
	wsdlFile << "<?xml version=\"1.0\"?>" << endl;

	wsdlFile << "<definitions name=\"StockQuote\" "
		<< "targetNamespace=\"http://example.com/stockquote.wsdl\" xmlns:tns =\"http://example.com/stockquote.wsdl\" "
		<< "xmlns:xsd1=\"http://example.com/stockquote.xsd\" "
		<< "xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\" "
 		<< "xmlns=\"http://schemas.xmlsoap.org/wsdl/ \"> " << endl;

	wsdlFile << "<service name = \"" << name << "\"> " << endl
		<< "<documentation>" << documentation << "</documentation>" << endl
		<< "<port name = \"" << portType << "\" binding = \"tns:Trigger\">" << endl
		<< "<soap:address location = \"" << url << "\"/>" << endl
		<< "</port>"  << endl
		<< "</service>" << endl;

	wsdlFile << "<portType name =\"" << portType << "\">" << endl
		<< "<operation name =\"" << operation << "\">" << endl
		<< "<input message =\"" << inputMessage << "\"/>" << endl
		<< "<output message =\"" << outputMessage << "\"/>" << endl
		<< "</operation>" << endl
		<< "</portType>" << endl;
		
	wsdlFile << "<message name = \"" << inputMessage << "\">" << endl
		<< "<part name = \"body\" element=\"xsd1:" << inputName << "\"/>" << endl
		<< "</message>" << endl;

	wsdlFile << "<message name = \"" << outputMessage << "\">" << endl
		<< "<part name = \"body\" element=\"xsd1:" << outputName << "\"/>" << endl
		<< "</message>" << endl;

	wsdlFile << "<types>" << endl
		<< "<schema targetNamespace = \"http://example.com/stockquote.xsd\" xmlns = \"http://www.w3.org/2000/10/XMLSchema\">" << endl
		<< "<element name = \"" << inputName << "\">" << endl
		<< "<complexType>" << endl
		<< "<all>" << endl
		<< "<element name =\"" << inputElementName << "\" type =\"" << inputDataType << "\" value =\"" << inputValue << "\"/>" << endl
		<< "</all>" << endl
		<< "</complexType>" << endl
		<< "</element>" << endl
		<< "<element name =\"" << outputName << "\">" << endl
		<< "<complexType>" << endl
		<< "<all>" << endl
		<< "<element name =\"" << outputElementName << "\" type =\"" << outputDataType << "\" value =\"" << outputValue << "\"/>" << endl
		<< "</all>" << endl
		<< "</complexType>" << endl
		<< "</element>" << endl
		<< "</schema>" << endl
		<< "</types>" << endl;
		
	wsdlFile << "</definitions>";
	wsdlFile.close();
}

void service::setName(string Name)
{
	name = Name;
}

string service::getName()
{
	return name;
}

void service::setDocumentation(string Documentation)
{
	documentation = Documentation;
}

string service::getDocumentation()
{
	return documentation;
}

void service::setUrl(string Url)
{
	url = Url;
}

string service::getUrl()
{
	return url;
}

void service::setPortType(string PortType)
{
	portType = PortType;
}

string service::getPortType()
{
	return portType;
}

void service::setOperation(string Operation)
{
	operation = Operation;
}

string service::getOperation()
{
	return operation;
}

void service::setInputMessage(string InputMessage)
{
	inputMessage = InputMessage;
}

string service::getInputMessage()
{
	return inputMessage;
}

void service::setInputName(string InputName)
{
	inputName = InputName;
}

string service::getInputName()
{
	return inputName;
}

void service::setInputElementName(string InputElementName)
{
	inputElementName = InputElementName;
}

string service::getInputElementName()
{
	return inputElementName;
}

void service::setInputDataType(string InputDataType)
{
	inputDataType = InputDataType;
}

string service::getInputDatatType()
{
	return inputDataType;
}

void service::setInputValue(string InputValue)
{
	inputValue = InputValue;
}

string service::getInputValue()
{
	return inputValue;
}

void service::setOutputMessage(string OutputMessage)
{
	outputMessage = OutputMessage;
}

string service::getOutputMessage()
{
	return outputMessage;
}

void service::setOutputName(string OutputName)
{
	outputName = OutputName;
}

string service::getOutputName()
{
	return outputName;
}

void service::setOutputElementName(string OutputElementName)
{
	outputElementName = OutputElementName;
}

string service::getOutputElementName()
{
	return outputElementName;
}

void service::setOutputDataType(string OutputDataType)
{
	outputDataType = OutputDataType;
}

string service::getOutputDataType()
{
	return outputDataType;
}

void service::setOutputValue(string OutputValue)
{
	outputValue = OutputValue;
}

string service::getOutputValue()
{
	return outputValue;
}
