#include "XmlIrrigationActionReader.h"
#include "pugixml.hpp"

using namespace std;
using namespace pugi;


XmlIrrigationActionReader::XmlIrrigationActionReader() {
}

XmlIrrigationActionReader::~XmlIrrigationActionReader() {
}

IrrigationActionDTO XmlIrrigationActionReader::load(const string& text) const {
	const char* tagName = "irrigation";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'irrigation' element tag not found");
	}

	IrrigationActionDTO result;
	xml_node tmpNode;

	if ((tmpNode = node.child("action")) != nullptr) {
		result.action.reset(new string(tmpNode.text().as_string()));
	}

	if ((tmpNode = node.child("adjustment")) != nullptr) {
		result.adjustment.reset(new unsigned(tmpNode.text().as_uint()));
	}

	if ((tmpNode = node.child("program-id")) != nullptr) {
		result.programId.reset(new IdType(IdType::from_string(tmpNode.text().as_string())));
	}

	if ((tmpNode = node.child("runtimes")) != nullptr) {
		result.runTimeDtoList.reset(new list<RunTimeDTO>(loadRunTimeList(tmpNode)));
	}

	return result;
}
