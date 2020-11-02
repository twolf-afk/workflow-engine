#ifndef SERVICE_H

#define SERVICE_H

#include <string>

using namespace std;

class service
{

private:

	string name;
	string documentation;
	string url;
	
	string portType;
	string operation;

	string inputMessage;
	string inputName;
	string inputElementName;
	string inputDataType;
	string inputValue;

	string outputMessage;
	string outputName;
	string outputElementName;
	string outputDataType;
	string outputValue;

public:

	service();

	void createServiceAsXml(string filename);

	void setName(string Name);
	string getName();

	void setDocumentation(string Documentation);
	string getDocumentation();

	void setUrl(string Url);
	string getUrl();


	void setPortType(string PortType);
	string getPortType();

	void setOperation(string Operation);
	string getOperation();


	void setInputMessage(string InputMessage);
	string getInputMessage();

	void setInputName(string InputName);
	string getInputName();

	void setInputElementName(string InputElementName);
	string getInputElementName();

	void setInputDataType(string InputDataType);
	string getInputDatatType();

	void setInputValue(string Value);
	string getInputValue();


	void setOutputMessage(string OutputMessage);
	string getOutputMessage();

	void setOutputName(string OutputName);
	string getOutputName();

	void setOutputElementName(string OutputElementName);
	string getOutputElementName();

	void setOutputDataType(string OutputDataType);
	string getOutputDataType();

	void setOutputValue(string OutputValue);
	string getOutputValue();

};

#endif