#include "serviceInput.h"

serviceInput::serviceInput()
{
	nodeId = "";
	datatype = "";
	value = "";
}

std::string serviceInput::getNodeId()
{
	return nodeId;
}

void serviceInput::setNodeId(std::string nodeID)
{
	nodeId = nodeID;
}

std::string serviceInput::getDatatype()
{
	return datatype;
}

void serviceInput::setDatatype(std::string Datatype)
{
	datatype = Datatype;
}

std::string serviceInput::getValue()
{
	return value;
}

void serviceInput::setValue(std::string Value)
{
	value = Value;
}
