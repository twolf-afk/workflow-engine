#ifndef SERVICEARGUMENTTEMPLATE_H

#define SERVICEARGUMENTTEMPLATE_H

#include <string>

class serviceArgumentTemplate
{

private:

	std::string name;
	std::string datatype;
	std::string value;

public:

	std::string createServiceArgumentAsXmlString();

	void setName(std::string Name);
	std::string getName();

	void setDataType(std::string DataType);
	std::string getDatatType();

	void setValue(std::string Value);
	std::string getValue();

};

#endif