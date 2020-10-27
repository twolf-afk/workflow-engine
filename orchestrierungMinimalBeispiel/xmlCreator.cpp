#include "xmlCreator.h"

#include <fstream>

void xmlCreator::writeStringAsXML(std::string string, std::string fileName)
{
	std::ofstream out(fileName);
	out << string;
	out.close();
}