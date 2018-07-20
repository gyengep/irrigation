#include <ReaderWriter/XmlReader.h>
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
		throw XMLParseException(result.description());
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
			programNode = programNode.next_sibling("program");
		}

		document.setPrograms(programs.release());
	}
}

void XmlReader::loadProgram(const xml_node& node, ProgramDTO& program, bool isIdRequired) const {
	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		program.setId(idAttribute.as_uint());
	} else if (isIdRequired) {
		throw RequiredAttributeMissing("The 'id' attribute is missing");
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
			runTimeNode = runTimeNode.next_sibling("runtime");
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
			startTimeNode = startTimeNode.next_sibling("starttime");
		}

		program.setStartTimes(startTimes.release());
	}
}

void XmlReader::loadScheduler(const xml_node& node, SpecifiedSchedulerDTO& scheduler) const {
	xml_attribute typeAttribute;
	if ((typeAttribute = node.attribute("type")) != nullptr) {
		if (strcmp(typeAttribute.as_string(), "specified") != 0) {
			throw invalid_argument(string("XmlReader::loadScheduler(): invalid SchedulerType: ") + typeAttribute.as_string());
		}
	}

	xml_node tmpNode;

	if ((tmpNode = node.child("adjustment")) != nullptr) {
		scheduler.setAdjustment(tmpNode.text().as_uint());
	}

	if ((tmpNode = node.child("days")) != nullptr) {
		unique_ptr<list<bool>> values(new list<bool>());

		xml_node dayNode = tmpNode.child("day");
		while (dayNode) {
			values->push_back(dayNode.text().as_bool());
			dayNode = dayNode.next_sibling("day");
		}

		scheduler.setValues(values.release());
	}
}

void XmlReader::loadRunTime(const xml_node& node, RunTimeDTO& runTime, bool isIdRequired) const {
	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		runTime.setId(idAttribute.as_uint());
	} else if (isIdRequired) {
		throw RequiredAttributeMissing("The 'id' attribute is missing");
	}

	xml_text nodeText;
	if ((nodeText = node.text()) != nullptr) {
		runTime.setValue(nodeText.as_uint());
	}
}

void XmlReader::loadStartTime(const xml_node& node, StartTimeDTO& startTime, bool isIdRequired) const {
	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		startTime.setId(idAttribute.as_uint());
	} else if (isIdRequired) {
		throw RequiredAttributeMissing("The 'id' attribute is missing");
	}

	xml_text nodeText;
	if ((nodeText = node.text()) != nullptr) {
		startTime.setValue(nodeText.as_uint());
	}
}

///////////////////////////////////////////////////////////////////////////////

DocumentDTO XmlReader::loadDocument(const string& text) const {
	const char* tagName = "irrigation";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'irrigation' element tag not found");
	}

	DocumentDTO document;
	loadDocument(node, document);
	return document;
}

ProgramDTO XmlReader::loadProgram(const string& text) const {
	const char* tagName = "program";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'program' element tag not found");
	}

	ProgramDTO program;
	loadProgram(node, program, false);
	return program;
}

RunTimeDTO XmlReader::loadRunTime(const string& text) const {
	const char* tagName = "runtime";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'runtime' element tag not found");
	}

	RunTimeDTO runTime;
	loadRunTime(node, runTime, false);
	return runTime;
}

StartTimeDTO XmlReader::loadStartTime(const string& text) const {
	const char* tagName = "starttime";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'starttime' element tag not found");
	}

	StartTimeDTO startTime;
	loadStartTime(node, startTime, false);
	return startTime;
}

SpecifiedSchedulerDTO XmlReader::loadSpecifiedScheduler(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	SpecifiedSchedulerDTO scheduler;
	loadScheduler(node, scheduler);
	return scheduler;
}
