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
		throw XMLParseException(result.description());
	}
}

///////////////////////////////////////////////////////////////////////////////

DocumentDTO XmlReader::loadDocument(const xml_node& node) const {
	DocumentDTO document;
	xml_node programListNode;
	if ((programListNode = node.child("programs")) != nullptr) {
		list<ProgramDTO> programs;

		xml_node programNode = programListNode.child("program");
		while (programNode) {
			programs.push_back(loadProgram(programNode));
			programNode = programNode.next_sibling("program");
		}

		document.setPrograms(move(programs));
	}

	return document;
}

ProgramDTO XmlReader::loadProgram(const xml_node& node) const {
	ProgramDTO program;
	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		program.setId(idAttribute.as_uint());
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
			program.setSpecifiedScheduler(loadScheduler(schedulerNode));
		}
	}

	if ((tmpNode = node.child("runtimes")) != nullptr) {
		list<RunTimeDTO> runTimes;

		xml_node runTimeNode = tmpNode.child("runtime");
		while (runTimeNode) {
			runTimes.push_back(loadRunTime(runTimeNode));
			runTimeNode = runTimeNode.next_sibling("runtime");
		}

		program.setRunTimes(move(runTimes));
	}

	if ((tmpNode = node.child("starttimes")) != nullptr) {
		list<StartTimeDTO> startTimes;

		xml_node startTimeNode = tmpNode.child("starttime");
		while (startTimeNode) {
			startTimes.push_back(loadStartTime(startTimeNode));
			startTimeNode = startTimeNode.next_sibling("starttime");
		}

		program.setStartTimes(move(startTimes));
	}

	return program;
}

SpecifiedSchedulerDTO XmlReader::loadScheduler(const xml_node& node) const {
	SpecifiedSchedulerDTO scheduler;
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
		list<bool> values;

		xml_node dayNode = tmpNode.child("day");
		while (dayNode) {
			values.push_back(dayNode.text().as_bool());
			dayNode = dayNode.next_sibling("day");
		}

		scheduler.setValues(move(values));
	}

	return scheduler;
}

RunTimeDTO XmlReader::loadRunTime(const xml_node& node) const {
	RunTimeDTO runTime;
	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		runTime.setId(idAttribute.as_uint());
	}

	xml_node tmpNode;
	if ((tmpNode = node.child("minute")) != nullptr) {
		runTime.setMinutes(tmpNode.text().as_uint());
	}

	if ((tmpNode = node.child("second")) != nullptr) {
		runTime.setSeconds(tmpNode.text().as_uint());
	}

	return runTime;
}

StartTimeDTO XmlReader::loadStartTime(const xml_node& node) const {
	StartTimeDTO startTime;
	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		startTime.setId(idAttribute.as_uint());
	}

	xml_node tmpNode;
	if ((tmpNode = node.child("hour")) != nullptr) {
		startTime.setHour(tmpNode.text().as_uint());
	}

	if ((tmpNode = node.child("minute")) != nullptr) {
		startTime.setMinute(tmpNode.text().as_uint());
	}

	return startTime;
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

	return loadDocument(node);
}

ProgramDTO XmlReader::loadProgram(const string& text) const {
	const char* tagName = "program";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'program' element tag not found");
	}

	return loadProgram(node);
}

RunTimeDTO XmlReader::loadRunTime(const string& text) const {
	const char* tagName = "runtime";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'runtime' element tag not found");
	}

	return loadRunTime(node);
}

StartTimeDTO XmlReader::loadStartTime(const string& text) const {
	const char* tagName = "starttime";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'starttime' element tag not found");
	}

	return loadStartTime(node);
}

SpecifiedSchedulerDTO XmlReader::loadSpecifiedScheduler(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	return loadScheduler(node);
}
