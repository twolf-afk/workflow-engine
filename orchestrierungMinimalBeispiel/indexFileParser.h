#ifndef INDEXFILEPARSER_H

#define INDEXFILEPARSER_H

#include <xercesc/dom/DOM.hpp>

#include <string>
#include <map>

class indexFileParser
{

private:

	xercesc_3_2::DOMElement* rootElement;


	void initXmlParserGetDocumentGetRootElement(std::string fileName);

public:

	indexFileParser(std::string filename);

	std::map<int, std::string> getServices();

};

#endif
