#include "XmlReader.h"
#include <sstream>
#include <string.h>
#include "pugixml.hpp"
#include "DTO/DocumentDTO.h"
#include "DTO/ProgramDTO.h"
#include "DTO/RunTimeDTO.h"
#include "DTO/StartTimeDTO.h"
#include "DTO/WeeklySchedulerDTO.h"

using namespace std;
using namespace pugi;



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
		document.setPrograms(loadProgramList(programListNode));
	}

	return document;
}

list<ProgramDTO> XmlReader::loadProgramList(const xml_node& node) const {
	list<ProgramDTO> programList;

	xml_node programNode = node.child("program");
	while (programNode) {
		programList.push_back(loadProgram(programNode));
		programNode = programNode.next_sibling("program");
	}

	return programList;
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

	if ((tmpNode = node.child("disabled")) != nullptr) {
		program.setDisabled(tmpNode.text().as_bool());
	}

	if ((tmpNode = node.child("adjustment")) != nullptr) {
		program.setAdjustment(tmpNode.text().as_uint());
	}

	if ((tmpNode = node.child("schedulertype")) != nullptr) {
		program.setSchedulerType(tmpNode.text().as_string());
	}

	if ((tmpNode = node.child("schedulers")) != nullptr) {
		xml_node schedulerNode;

		if ((schedulerNode = tmpNode.find_child_by_attribute("scheduler", "type", "periodic")) != nullptr) {
			program.setPeriodicScheduler(loadPeriodicScheduler(schedulerNode));
		}

		if ((schedulerNode = tmpNode.find_child_by_attribute("scheduler", "type", "weekly")) != nullptr) {
			program.setWeeklyScheduler(loadWeeklyScheduler(schedulerNode));
		}

		if ((schedulerNode = tmpNode.find_child_by_attribute("scheduler", "type", "every_day")) != nullptr) {
			program.setEveryDayScheduler(loadEveryDayScheduler(schedulerNode));
		}
	}

	if ((tmpNode = node.child("runtimes")) != nullptr) {
		program.setRunTimes(loadRunTimeList(tmpNode));
	}

	if ((tmpNode = node.child("starttimes")) != nullptr) {
		program.setStartTimes(loadStartTimeList(tmpNode));
	}

	return program;
}

PeriodicSchedulerDTO XmlReader::loadPeriodicScheduler(const xml_node& node) const {
	PeriodicSchedulerDTO scheduler;

	xml_attribute typeAttribute;
	if ((typeAttribute = node.attribute("type")) != nullptr) {
		if (strcmp(typeAttribute.as_string(), "periodic") != 0) {
			throw invalid_argument(string("XmlReader::loadScheduler(): invalid SchedulerType: ") + typeAttribute.as_string());
		}
	}

	xml_node tmpNode;

	if ((tmpNode = node.child("days")) != nullptr) {
		list<bool> values;

		xml_node dayNode = tmpNode.child("day");
		while (dayNode) {
			values.push_back(dayNode.text().as_bool());
			dayNode = dayNode.next_sibling("day");
		}

		scheduler.setValues(move(values));
	}

	if ((tmpNode = node.child("periodStartDate")) != nullptr) {
		xml_node dateNode;

		if ((dateNode = tmpNode.child("year")) != nullptr) {
			scheduler.setPeriodStartYear(dateNode.text().as_uint());
		}

		if ((dateNode = tmpNode.child("month")) != nullptr) {
			scheduler.setPeriodStartMonth(dateNode.text().as_uint());
		}

		if ((dateNode = tmpNode.child("day")) != nullptr) {
			scheduler.setPeriodStartDay(dateNode.text().as_uint());
		}
	}

	return scheduler;
}

WeeklySchedulerDTO XmlReader::loadWeeklyScheduler(const xml_node& node) const {
	WeeklySchedulerDTO scheduler;
	xml_attribute typeAttribute;
	if ((typeAttribute = node.attribute("type")) != nullptr) {
		if (strcmp(typeAttribute.as_string(), "weekly") != 0) {
			throw invalid_argument(string("XmlReader::loadScheduler(): invalid SchedulerType: ") + typeAttribute.as_string());
		}
	}

	xml_node tmpNode;

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

EveryDaySchedulerDTO XmlReader::loadEveryDayScheduler(const xml_node& node) const {
	EveryDaySchedulerDTO scheduler;
	xml_attribute typeAttribute;
	if ((typeAttribute = node.attribute("type")) != nullptr) {
		if (strcmp(typeAttribute.as_string(), "every_day") != 0) {
			throw invalid_argument(string("XmlReader::loadScheduler(): invalid SchedulerType: ") + typeAttribute.as_string());
		}
	}

	return scheduler;
}

list<RunTimeDTO> XmlReader::loadRunTimeList(const xml_node& node) const {
	list<RunTimeDTO> runTimesList;

	xml_node runTimeNode = node.child("runtime");
	while (runTimeNode) {
		runTimesList.push_back(loadRunTime(runTimeNode));
		runTimeNode = runTimeNode.next_sibling("runtime");
	}

	return runTimesList;
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

list<StartTimeDTO> XmlReader::loadStartTimeList(const xml_node& node) const {
	list<StartTimeDTO> startTimeList;

	xml_node startTimeNode = node.child("starttime");
	while (startTimeNode) {
		startTimeList.push_back(loadStartTime(startTimeNode));
		startTimeNode = startTimeNode.next_sibling("starttime");
	}

	return startTimeList;
}

StartTimeDTO XmlReader::loadStartTime(const xml_node& node) const {
	StartTimeDTO startTime;
	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		startTime.setId(idAttribute.as_uint());
	}

	xml_node tmpNode;
	if ((tmpNode = node.child("hour")) != nullptr) {
		startTime.setHours(tmpNode.text().as_uint());
	}

	if ((tmpNode = node.child("minute")) != nullptr) {
		startTime.setMinutes(tmpNode.text().as_uint());
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

list<ProgramDTO> XmlReader::loadProgramList(const string& text) const {
	const char* tagName = "programs";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'programs' element tag not found");
	}

	return loadProgramList(node);
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

list<RunTimeDTO> XmlReader::loadRunTimeList(const string& text) const {
	const char* tagName = "runtimes";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'runtimes' element tag not found");
	}

	return loadRunTimeList(node);
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

list<StartTimeDTO> XmlReader::loadStartTimeList(const string& text) const {
	const char* tagName = "starttimes";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'starttimes' element tag not found");
	}

	return loadStartTimeList(node);
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

PeriodicSchedulerDTO XmlReader::loadPeriodicScheduler(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	return loadPeriodicScheduler(node);
}

WeeklySchedulerDTO XmlReader::loadWeeklyScheduler(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	return loadWeeklyScheduler(node);
}

EveryDaySchedulerDTO XmlReader::loadEveryDayScheduler(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	return loadEveryDayScheduler(node);
}
