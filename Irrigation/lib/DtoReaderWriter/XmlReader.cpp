#include "XmlReader.h"
#include <pugixml/pugixml.hpp>
#include <cstring>

using namespace std;
using namespace pugi;



void XmlReader::loadFromString(xml_document* doc, const string& text) {
	xml_parse_result result = doc->load_string(text.c_str());

	if (status_ok != result.status) {
		throw XMLParseException(result.description());
	}
}

///////////////////////////////////////////////////////////////////////////////

DocumentDto XmlReader::loadDocument(const xml_node& node) const {
	DocumentDto document;

	xml_node programListNode;
	if ((programListNode = node.child("programs")) != nullptr) {
		document.setPrograms(loadProgramList(programListNode));
	}

	return document;
}

list<ProgramDto> XmlReader::loadProgramList(const xml_node& node) const {
	list<ProgramDto> programList;

	xml_node programNode = node.child("program");
	while (programNode) {
		programList.push_back(loadProgram(programNode));
		programNode = programNode.next_sibling("program");
	}

	return programList;
}

ProgramDto XmlReader::loadProgram(const xml_node& node) const {
	ProgramDto program;
	xml_attribute idAttribute;
	if ((idAttribute = node.attribute("id")) != nullptr) {
		program.setId(idAttribute.as_uint());
	}

	xml_node tmpNode;

	if ((tmpNode = node.child("name")) != nullptr) {
		program.setName(tmpNode.text().as_string());
	}

	if ((tmpNode = node.child("enabled")) != nullptr) {
		program.setEnabled(tmpNode.text().as_bool());
	}

	if ((tmpNode = node.child("adjustment")) != nullptr) {
		program.setAdjustment(tmpNode.text().as_uint());
	}

	if ((tmpNode = node.child("schedulertype")) != nullptr) {
		program.setSchedulerType(tmpNode.text().as_string());
	}

	if ((tmpNode = node.child("schedulers")) != nullptr) {
		program.setSchedulers(loadSchedulers(tmpNode));
	}

	if ((tmpNode = node.child("runtimes")) != nullptr) {
		program.setRunTimes(loadRunTimeList(tmpNode));
	}

	if ((tmpNode = node.child("starttimes")) != nullptr) {
		program.setStartTimes(loadStartTimeList(tmpNode));
	}

	return program;
}

SchedulersDto XmlReader::loadSchedulers(const xml_node& node) const {
	SchedulersDto schedulers;
	xml_node schedulerNode;

	if ((schedulerNode = node.find_child_by_attribute("scheduler", "type", "every-day")) != nullptr) {
		schedulers.setEveryDayScheduler(loadEveryDayScheduler(schedulerNode));
	}

	if ((schedulerNode = node.find_child_by_attribute("scheduler", "type", "hot-weather")) != nullptr) {
		schedulers.setHotWeatherScheduler(loadHotWeatherScheduler(schedulerNode));
	}

	if ((schedulerNode = node.find_child_by_attribute("scheduler", "type", "temperature-dependent")) != nullptr) {
		schedulers.setTemperatureDependentScheduler(loadTemperatureDependentScheduler(schedulerNode));
	}

	if ((schedulerNode = node.find_child_by_attribute("scheduler", "type", "weekly")) != nullptr) {
		schedulers.setWeeklyScheduler(loadWeeklyScheduler(schedulerNode));
	}

	return schedulers;
}

EveryDaySchedulerDto XmlReader::loadEveryDayScheduler(const xml_node& node) const {
	EveryDaySchedulerDto scheduler;
	xml_attribute typeAttribute;
	if ((typeAttribute = node.attribute("type")) != nullptr) {
		if (strcmp(typeAttribute.as_string(), "every-day") != 0) {
			throw invalid_argument(string("XmlReader::loadScheduler(): invalid SchedulerType: ") + typeAttribute.as_string());
		}
	}

	return scheduler;
}

HotWeatherSchedulerDto XmlReader::loadHotWeatherScheduler(const xml_node& node) const {
	HotWeatherSchedulerDto scheduler;

	xml_attribute typeAttribute;
	if ((typeAttribute = node.attribute("type")) != nullptr) {
		if (strcmp(typeAttribute.as_string(), "hot-weather") != 0) {
			throw invalid_argument(string("XmlReader::loadScheduler(): invalid SchedulerType: ") + typeAttribute.as_string());
		}
	}

	xml_node tmpNode;
	if ((tmpNode = node.child("period")) != nullptr) {
		scheduler.setPeriodInSeconds(tmpNode.text().as_uint());
	}

	if ((tmpNode = node.child("temperature")) != nullptr) {
		scheduler.setMinTemperature(tmpNode.text().as_float());
	}

	return scheduler;
}

TemperatureDependentSchedulerDto XmlReader::loadTemperatureDependentScheduler(const xml_node& node) const {
	TemperatureDependentSchedulerDto scheduler;

	xml_attribute typeAttribute;
	if ((typeAttribute = node.attribute("type")) != nullptr) {
		if (strcmp(typeAttribute.as_string(), "temperature-dependent") != 0) {
			throw invalid_argument(string("XmlReader::loadScheduler(): invalid SchedulerType: ") + typeAttribute.as_string());
		}
	}

	xml_node tmpNode;
	if ((tmpNode = node.child("remaining-correction")) != nullptr) {
		scheduler.setRemainingCorrection(tmpNode.text().as_float());
	}

	if ((tmpNode = node.child("min-adjustment")) != nullptr) {
		scheduler.setMinAdjustment(tmpNode.text().as_uint());
	}

	if ((tmpNode = node.child("max-adjustment")) != nullptr) {
		scheduler.setMaxAdjustment(tmpNode.text().as_uint());
	}

	return scheduler;
}

WeeklySchedulerDto XmlReader::loadWeeklyScheduler(const xml_node& node) const {
	WeeklySchedulerDto scheduler;
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

list<RunTimeDto> XmlReader::loadRunTimeList(const xml_node& node) const {
	list<RunTimeDto> runTimesList;

	xml_node runTimeNode = node.child("runtime");
	while (runTimeNode) {
		runTimesList.push_back(loadRunTime(runTimeNode));
		runTimeNode = runTimeNode.next_sibling("runtime");
	}

	return runTimesList;
}

RunTimeDto XmlReader::loadRunTime(const xml_node& node) const {
	RunTimeDto runTime;
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

list<StartTimeDto> XmlReader::loadStartTimeList(const xml_node& node) const {
	list<StartTimeDto> startTimeList;

	xml_node startTimeNode = node.child("starttime");
	while (startTimeNode) {
		startTimeList.push_back(loadStartTime(startTimeNode));
		startTimeNode = startTimeNode.next_sibling("starttime");
	}

	return startTimeList;
}

StartTimeDto XmlReader::loadStartTime(const xml_node& node) const {
	StartTimeDto startTime;
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

DocumentDto XmlReader::loadDocument(const string& text) const {
	const char* tagName = "irrigation";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'irrigation' element tag not found");
	}

	return loadDocument(node);
}

list<ProgramDto> XmlReader::loadProgramList(const string& text) const {
	const char* tagName = "programs";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'programs' element tag not found");
	}

	return loadProgramList(node);
}

ProgramDto XmlReader::loadProgram(const string& text) const {
	const char* tagName = "program";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'program' element tag not found");
	}

	return loadProgram(node);
}

list<RunTimeDto> XmlReader::loadRunTimeList(const string& text) const {
	const char* tagName = "runtimes";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'runtimes' element tag not found");
	}

	return loadRunTimeList(node);
}

RunTimeDto XmlReader::loadRunTime(const string& text) const {
	const char* tagName = "runtime";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'runtime' element tag not found");
	}

	return loadRunTime(node);
}

list<StartTimeDto> XmlReader::loadStartTimeList(const string& text) const {
	const char* tagName = "starttimes";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'starttimes' element tag not found");
	}

	return loadStartTimeList(node);
}

StartTimeDto XmlReader::loadStartTime(const string& text) const {
	const char* tagName = "starttime";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'starttime' element tag not found");
	}

	return loadStartTime(node);
}

SchedulersDto XmlReader::loadSchedulers(const std::string& text) const {
	const char* tagName = "schedulers";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'schedulers' element tag not found");
	}

	return loadSchedulers(node);
}

EveryDaySchedulerDto XmlReader::loadEveryDayScheduler(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	return loadEveryDayScheduler(node);
}

HotWeatherSchedulerDto XmlReader::loadHotWeatherScheduler(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	return loadHotWeatherScheduler(node);
}

TemperatureDependentSchedulerDto XmlReader::loadTemperatureDependentScheduler(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	return loadTemperatureDependentScheduler(node);
}

WeeklySchedulerDto XmlReader::loadWeeklyScheduler(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	return loadWeeklyScheduler(node);
}
