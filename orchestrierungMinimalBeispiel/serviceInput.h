#ifndef SERVICEINPUT_H

#define SERVICEINPUT_H

#include <string>

class serviceInput
{

private:

	std::string nodeId;
	std::string datatype;
	std::string value;

public:

	serviceInput();

	std::string getNodeId();
	void setNodeId(std::string nodeID);

	std::string getDatatype();
	void setDatatype(std::string Datatype);

	std::string getValue();
	void setValue(std::string Value);
};

#endif
