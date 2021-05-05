#include "serviceArgumentTemplate.h"

std::string serviceArgumentTemplate::createServiceArgumentAsXmlString()
{
    return "<element name =\"" + name + "\" type =\"" + datatype + "\" value =\"" + value + "\"/>";
}

void serviceArgumentTemplate::setName(std::string Name)
{
    name = Name;
}

std::string serviceArgumentTemplate::getName()
{
    return name;
}

void serviceArgumentTemplate::setDataType(std::string DataType)
{
    datatype = DataType;
}

std::string serviceArgumentTemplate::getDatatType()
{
    return datatype;
}

void serviceArgumentTemplate::setValue(std::string Value)
{
    value = Value;
}

std::string serviceArgumentTemplate::getValue()
{
    return value;
}
