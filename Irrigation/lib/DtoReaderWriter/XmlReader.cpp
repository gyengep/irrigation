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

DocumentDto XmlReader::loadDocumentDto(const xml_node& node) const {
	DocumentDto documentDto;

	xml_node programListNode;
	if ((programListNode = node.child("programs")) != nullptr) {
		documentDto.setPrograms(loadProgramDtoList(programListNode));
	}

	return documentDto;
}

ProgramDtoList XmlReader::loadProgramDtoList(const xml_node& node) const {
	ProgramDtoList programDtoList;

	xml_node programNode = node.child("program");
	while (programNode) {
		programDtoList.push_back(loadProgramDto(programNode));
		programNode = programNode.next_sibling("program");
	}

	return programDtoList;
}

ProgramDto XmlReader::loadProgramDto(const xml_node& node) const {
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
		program.setSchedulers(loadSchedulersDto(tmpNode));
	}

	if ((tmpNode = node.child("runtimes")) != nullptr) {
		program.setRunTimes(loadRunTimeDtoList(tmpNode));
	}

	if ((tmpNode = node.child("starttimes")) != nullptr) {
		program.setStartTimes(loadStartTimeDtoList(tmpNode));
	}

	return program;
}

SchedulersDto XmlReader::loadSchedulersDto(const xml_node& node) const {
	SchedulersDto schedulers;
	xml_node schedulerNode;

	if ((schedulerNode = node.find_child_by_attribute("scheduler", "type", "every-day")) != nullptr) {
		schedulers.setEveryDayScheduler(loadEveryDaySchedulerDto(schedulerNode));
	}

	if ((schedulerNode = node.find_child_by_attribute("scheduler", "type", "hot-weather")) != nullptr) {
		schedulers.setHotWeatherScheduler(loadHotWeatherSchedulerDto(schedulerNode));
	}

	if ((schedulerNode = node.find_child_by_attribute("scheduler", "type", "temperature-dependent")) != nullptr) {
		schedulers.setTemperatureDependentScheduler(loadTemperatureDependentSchedulerDto(schedulerNode));
	}

	if ((schedulerNode = node.find_child_by_attribute("scheduler", "type", "weekly")) != nullptr) {
		schedulers.setWeeklyScheduler(loadWeeklySchedulerDto(schedulerNode));
	}

	return schedulers;
}

EveryDaySchedulerDto XmlReader::loadEveryDaySchedulerDto(const xml_node& node) const {
	EveryDaySchedulerDto schedulerDto;
	xml_attribute typeAttribute;
	if ((typeAttribute = node.attribute("type")) != nullptr) {
		if (strcmp(typeAttribute.as_string(), "every-day") != 0) {
			throw invalid_argument(string("XmlReader::loadScheduler(): invalid SchedulerType: ") + typeAttribute.as_string());
		}
	}

	return schedulerDto;
}

HotWeatherSchedulerDto XmlReader::loadHotWeatherSchedulerDto(const xml_node& node) const {
	HotWeatherSchedulerDto schedulerDto;

	xml_attribute typeAttribute;
	if ((typeAttribute = node.attribute("type")) != nullptr) {
		if (strcmp(typeAttribute.as_string(), "hot-weather") != 0) {
			throw invalid_argument(string("XmlReader::loadScheduler(): invalid SchedulerType: ") + typeAttribute.as_string());
		}
	}

	xml_node tmpNode;
	if ((tmpNode = node.child("period")) != nullptr) {
		schedulerDto.setPeriodInSeconds(tmpNode.text().as_uint());
	}

	if ((tmpNode = node.child("temperature")) != nullptr) {
		schedulerDto.setMinTemperature(tmpNode.text().as_float());
	}

	return schedulerDto;
}

TemperatureDependentSchedulerDto XmlReader::loadTemperatureDependentSchedulerDto(const xml_node& node) const {
	TemperatureDependentSchedulerDto schedulerDto;

	xml_attribute typeAttribute;
	if ((typeAttribute = node.attribute("type")) != nullptr) {
		if (strcmp(typeAttribute.as_string(), "temperature-dependent") != 0) {
			throw invalid_argument(string("XmlReader::loadScheduler(): invalid SchedulerType: ") + typeAttribute.as_string());
		}
	}

	xml_node tmpNode;
	if ((tmpNode = node.child("remaining-correction")) != nullptr) {
		schedulerDto.setRemainingCorrection(tmpNode.text().as_float());
	}

	if ((tmpNode = node.child("min-adjustment")) != nullptr) {
		schedulerDto.setMinAdjustment(tmpNode.text().as_uint());
	}

	if ((tmpNode = node.child("max-adjustment")) != nullptr) {
		schedulerDto.setMaxAdjustment(tmpNode.text().as_uint());
	}

	return schedulerDto;
}

WeeklySchedulerDto XmlReader::loadWeeklySchedulerDto(const xml_node& node) const {
	WeeklySchedulerDto schedulerDto;
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

		schedulerDto.setValues(move(values));
	}

	return schedulerDto;
}

RunTimeDtoList XmlReader::loadRunTimeDtoList(const xml_node& node) const {
	RunTimeDtoList runTimesList;

	xml_node runTimeNode = node.child("runtime");
	while (runTimeNode) {
		runTimesList.push_back(loadRunTimeDto(runTimeNode));
		runTimeNode = runTimeNode.next_sibling("runtime");
	}

	return runTimesList;
}

RunTimeDto XmlReader::loadRunTimeDto(const xml_node& node) const {
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

StartTimeDtoList XmlReader::loadStartTimeDtoList(const xml_node& node) const {
	StartTimeDtoList startTimeList;

	xml_node startTimeNode = node.child("starttime");
	while (startTimeNode) {
		startTimeList.push_back(loadStartTimeDto(startTimeNode));
		startTimeNode = startTimeNode.next_sibling("starttime");
	}

	return startTimeList;
}

StartTimeDto XmlReader::loadStartTimeDto(const xml_node& node) const {
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

DocumentDto XmlReader::loadDocumentDto(const string& text) const {
	const char* tagName = "irrigation";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'irrigation' element tag not found");
	}

	return loadDocumentDto(node);
}

ProgramDtoList XmlReader::loadProgramDtoList(const string& text) const {
	const char* tagName = "programs";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'programs' element tag not found");
	}

	return loadProgramDtoList(node);
}

ProgramDto XmlReader::loadProgramDto(const string& text) const {
	const char* tagName = "program";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'program' element tag not found");
	}

	return loadProgramDto(node);
}

RunTimeDtoList XmlReader::loadRunTimeDtoList(const string& text) const {
	const char* tagName = "runtimes";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'runtimes' element tag not found");
	}

	return loadRunTimeDtoList(node);
}

RunTimeDto XmlReader::loadRunTimeDto(const string& text) const {
	const char* tagName = "runtime";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'runtime' element tag not found");
	}

	return loadRunTimeDto(node);
}

StartTimeDtoList XmlReader::loadStartTimeDtoList(const string& text) const {
	const char* tagName = "starttimes";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'starttimes' element tag not found");
	}

	return loadStartTimeDtoList(node);
}

StartTimeDto XmlReader::loadStartTimeDto(const string& text) const {
	const char* tagName = "starttime";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'starttime' element tag not found");
	}

	return loadStartTimeDto(node);
}

SchedulersDto XmlReader::loadSchedulersDto(const std::string& text) const {
	const char* tagName = "schedulers";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'schedulers' element tag not found");
	}

	return loadSchedulersDto(node);
}

EveryDaySchedulerDto XmlReader::loadEveryDaySchedulerDto(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	return loadEveryDaySchedulerDto(node);
}

HotWeatherSchedulerDto XmlReader::loadHotWeatherSchedulerDto(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	return loadHotWeatherSchedulerDto(node);
}

TemperatureDependentSchedulerDto XmlReader::loadTemperatureDependentSchedulerDto(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	return loadTemperatureDependentSchedulerDto(node);
}

WeeklySchedulerDto XmlReader::loadWeeklySchedulerDto(const string& text) const {
	const char* tagName = "scheduler";

	unique_ptr<xml_document> doc(new xml_document());
	loadFromString(doc.get(), text);

	const xml_node node = doc->child(tagName);

	if (node == nullptr) {
		throw RequiredTagMissing("The 'scheduler' element tag not found");
	}

	return loadWeeklySchedulerDto(node);
}
