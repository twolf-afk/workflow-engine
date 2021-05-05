#include "createServices.h"

#include "util.h"
#include "OpcUAClient.h"
#include "open62541Util.h"
#include "serviceTemplate.h"

#include <map>

std::map<std::string, std::string> browseNodes;
OpcUAClient client;
std::string url;

std::string getIdentifierFromNodeId(std::string nodeId) {

	std::vector<std::string> nodeIdParts = util::splitString(nodeId, ";");
	std::vector<std::string> identifierParts = util::splitString(nodeIdParts[1], "=");
	std::string identifier = identifierParts[1];

	return identifier;
}

void createAndSaveService(std::string nodeId) {

	serviceTemplate service = serviceTemplate();
	service.setDocumentation("documentation");
	service.setUrl(url);

	std::string portType = getIdentifierFromNodeId(nodeId);
	service.setPortType(portType);

	service.setInputMessage(portType);

	UA_BrowseResponse response = client.browseService(nodeId);

	for (size_t i = 0; i < response.resultsSize; i++) {
		for (size_t j = 0; j < response.results[i].referencesSize; j++) {

			UA_ReferenceDescription* ref = &(response.results[i].references[j]);

			std::string id = open62541Util::uaNodeIdToStdString(ref->nodeId.nodeId);
			std::string name = open62541Util::uaStringToStdString(ref->browseName.name);

			if (!(name.substr(0, 5).compare("input"))) {
				UA_Variant variant;
				UA_Variant_init(&variant);

				UA_NodeId node = open62541Util::createNodeId(id);
				variant = client.readValue(node, variant);
				std::string datatype = open62541Util::uaDataTypeToString(variant);
				
				service.createInputArgument(j, id, datatype);

			} else if (!(name.substr(0, 6).compare("output"))) {

				UA_Variant variant;
				UA_Variant_init(&variant);

				UA_NodeId node = open62541Util::createNodeId(id);

				variant = client.readValue(node, variant);
				std::string datatype = open62541Util::uaDataTypeToString(variant);

				service.createOutputArgument(j, id, datatype);
			}
		}
	}

	std::string serviceFileName = portType + ".wsdl";

	service.createServiceAsXml(portType + ".wsdl");
}

void createServices::browseServerAndCreateServicesRecursion(std::string nodeId) {

	UA_BrowseResponse response = client.browseService(nodeId);

	for (size_t i = 0; i < response.resultsSize; i++) {
		for (size_t j = 0; j < response.results[i].referencesSize; j++) {

			UA_ReferenceDescription* ref = &(response.results[i].references[j]);
			
			std::string id = open62541Util::uaNodeIdToStdString(ref->nodeId.nodeId);
			std::string name = open62541Util::uaStringToStdString(ref->browseName.name);

			if (browseNodes.count(id) < 1) {
				browseNodes.insert(std::make_pair(id, name));

				if (!(name.substr(0,3).compare("HWD"))) {
					createAndSaveService(id);
				}

				browseServerAndCreateServicesRecursion(id);
			}
		}
	}
}

void createServices::browseServerAndCreateServices(std::string Url) {

	url = Url;
	
	std::string rootNodeId = "ns=0;i=84";
	std::string rootName = "0, root";

	browseNodes = map<std::string, std::string>();

	browseNodes.insert(std::make_pair(rootNodeId, rootName));

	client.createAndConnectClient(url);

	browseServerAndCreateServicesRecursion(rootNodeId);
}