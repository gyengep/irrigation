#include "XmlReader.h"
#include <sstream>
#include <string.h>
#include "pugixml.hpp"
#include "DTO/DocumentDTO.h"
#include "DTO/ProgramDTO.h"
#include "DTO/RunTimeDTO.h"
#include "DTO/StartTimeDTO.h"
#include "DTO/SchedulersDTO.h"

using namespace std;
using namespace pugi;



XmlReader::XmlReader() {
}

XmlReader::~XmlReader() {
}

void XmlReader::loadFromString(xml_document* doc, const string& text) {
	xml_parse_result result = doc->load_string(text.c_str());

	if (status_ok != result.status) {
		throw XmlParsingError(result.description());
	}
}

///////////////////////////////////////////////////////////////////////////////

void XmlReader::loadDocument(const xml_node& node, DocumentDTO& document) const {
	xml_node programListNode;
	if ((programListNode = node.child("programs")) != nullptr) {
		unique_ptr<list<ProgramDTO>> programs(new list<ProgramDTO>());

		xml_node programNode = programListNode.child("program");
		while (programNode) {
			ProgramDTO program;
			loadProgram(programNode, program, true);
			programs->push_back(program);
			programNode = programNode.next_sibling();
		}

		document.setPrograms(programs.release());
	}
}

void XmlReader::loadProgram(const xml_node& node, ProgramDTO& program, bool idIsRequired) const {
	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		program.setId(idAttribute.as_uint());
	} else if (idIsRequired) {
		throw RequiredAttributeMissing("id");
	}

	xml_node tmpNode;

	if ((tmpNode = node.child("name")) != nullptr) {
		program.setName(tmpNode.text().as_string());
	}

	if ((tmpNode = node.child("schedulertype")) != nullptr) {
		program.setSchedulerType(tmpNode.text().as_string());
	}

	if ((tmpNode = node.child("schedulers")) != nullptr) {
		xml_node schedulerNode;

		if ((schedulerNode = tmpNode.find_child_by_attribute("scheduler", "type", "specified")) != nullptr) {
			SpecifiedSchedulerDTO scheduler;
			loadScheduler(schedulerNode, scheduler);
			program.setSpecifiedScheduler(scheduler);
		}
	}

	if ((tmpNode = node.child("runtimes")) != nullptr) {
		unique_ptr<list<RunTimeDTO>> runTimes(new list<RunTimeDTO>());

		xml_node runTimeNode = tmpNode.child("runtime");
		while (runTimeNode) {
			RunTimeDTO runTimeDTO;
			loadRunTime(runTimeNode, runTimeDTO, true);
			runTimes->push_back(runTimeDTO);
			runTimeNode = runTimeNode.next_sibling();
		}

		program.setRunTimes(runTimes.release());
	}

	if ((tmpNode = node.child("starttimes")) != nullptr) {
		unique_ptr<list<StartTimeDTO>> startTimes(new list<StartTimeDTO>());

		xml_node startTimeNode = tmpNode.child("starttime");
		while (startTimeNode) {
			StartTimeDTO startTimeDTO;
			loadStartTime(startTimeNode, startTimeDTO, true);
			startTimes->push_back(startTimeDTO);
			startTimeNode = startTimeNode.next_sibling();
		}

		program.setStartTimes(startTimes.release());
	}
}

void XmlReader::loadScheduler(const xml_node& node, SpecifiedSchedulerDTO& scheduler) const {
	xml_attribute typeAttribute;
	if ((typeAttribute = node.attribute("type")) != nullptr) {
		const char* type = "specified";
		if (strcmp(typeAttribute.as_string(), type) != 0) {
			throw BadSchedulerType(type);
		}
	}

	unique_ptr<list<bool>> values(new list<bool>());

	xml_node dayNode = node.child("day");
	while (dayNode) {
		values->push_back(dayNode.text().as_bool());
		dayNode = dayNode.next_sibling();
	}

	scheduler.setValues(values.release());
}

void XmlReader::loadRunTime(const xml_node& node, RunTimeDTO& runTime, bool idIsRequired) const {
	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		runTime.setId(idAttribute.as_uint());
	} else if (idIsRequired) {
		throw RequiredAttributeMissing("id");
	}

	xml_text nodeText;
	if ((nodeText = node.text()) != nullptr) {
		runTime.setValue(nodeText.as_uint());
	}
}

void XmlReader::loadStartTime(const xml_node& node, StartTimeDTO& startTime, bool idIsRequired) const {
	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		startTime.setId(idAttribute.as_uint());
	} else if (idIsRequired) {
		throw RequiredAttributeMissing("id");
	}

	xml_text nodeText;
	if ((nodeText = node.text()) != nullptr) {
		startTime.setValue(nodeText.as_uint());
	}
}

///////////////////////////////////////////////////////////////////////////////

void XmlReader::load(DocumentDTO& document, const string& text) const {
	const char* tagName = "irrigation";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing(tagName);
	}

	loadDocument(node, document);
}

void XmlReader::load(ProgramDTO& program, const string& text) const {
	const char* tagName = "program";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing(tagName);
	}

	loadProgram(node, program, false);
}

void XmlReader::load(RunTimeDTO& runTime, const string& text) const {
	const char* tagName = "runtime";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing(tagName);
	}

	loadRunTime(node, runTime, false);
}

void XmlReader::load(StartTimeDTO& startTime, const string& text) const {
	const char* tagName = "starttime";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing(tagName);
	}

	loadStartTime(node, startTime, false);
}

void XmlReader::load(SpecifiedSchedulerDTO& scheduler, const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing(tagName);
	}

	loadScheduler(node, scheduler);
}
