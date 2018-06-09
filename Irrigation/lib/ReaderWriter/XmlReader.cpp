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

void XmlReader::loadFromString(pugi::xml_document* doc, const std::string& text) {
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
			loadProgram(programNode, program);
			programs->push_back(program);
			programNode = programNode.next_sibling();
		}

		document.setPrograms(programs.release());
	}
}

void XmlReader::loadProgram(const xml_node& node, ProgramDTO& program) const {
	xml_node tmpNode;

	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		program.setId(idAttribute.as_uint());
	}

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
			loadRunTime(runTimeNode, runTimeDTO);
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
			loadStartTime(startTimeNode, startTimeDTO);
			startTimes->push_back(startTimeDTO);
			startTimeNode = startTimeNode.next_sibling();
		}

		program.setStartTimes(startTimes.release());
	}
}

void XmlReader::loadScheduler(const pugi::xml_node& node, SpecifiedSchedulerDTO& scheduler) const {
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

void XmlReader::loadRunTime(const xml_node& node, RunTimeDTO& runTime) const {
	xml_text nodeText;
	if ((nodeText = node.text()) != nullptr) {
		runTime.setValue(nodeText.as_uint());
	}

	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		runTime.setId(idAttribute.as_uint());
	}
}

void XmlReader::loadStartTime(const xml_node& node, StartTimeDTO& startTime) const {
	xml_text nodeText;
	if ((nodeText = node.text()) != nullptr) {
		startTime.setValue(nodeText.as_uint());
	}

	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		startTime.setId(idAttribute.as_uint());
	}

}

///////////////////////////////////////////////////////////////////////////////

void XmlReader::load(DocumentDTO& document, const std::string& text) const {
	const char* tagName = "irrigation";

	std::unique_ptr<pugi::xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing(tagName);
	}

	loadDocument(node, document);
}

void XmlReader::load(ProgramDTO& program, const std::string& text) const {
	const char* tagName = "program";

	std::unique_ptr<pugi::xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing(tagName);
	}

	loadProgram(node, program);
}

void XmlReader::load(RunTimeDTO& runTime, const std::string& text) const {
	const char* tagName = "runtime";

	std::unique_ptr<pugi::xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing(tagName);
	}

	loadRunTime(node, runTime);
}

void XmlReader::load(StartTimeDTO& startTime, const std::string& text) const {
	const char* tagName = "starttime";

	std::unique_ptr<pugi::xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing(tagName);
	}

	loadStartTime(node, startTime);
}

void XmlReader::load(SpecifiedSchedulerDTO& scheduler, const std::string& text) const {
	const char* tagName = "scheduler";

	std::unique_ptr<pugi::xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing(tagName);
	}

	loadScheduler(node, scheduler);
}
