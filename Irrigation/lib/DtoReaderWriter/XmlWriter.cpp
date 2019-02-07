#include "XmlWriter.h"
#include <sstream>
#include "pugixml.hpp"
#include "DTO/DocumentDTO.h"
#include "DTO/ProgramDTO.h"
#include "DTO/RunTimeDTO.h"
#include "DTO/StartTimeDTO.h"
#include "DTO/WeeklySchedulerDTO.h"

using namespace std;
using namespace pugi;



XmlWriter::XmlWriter(bool humanReadable) : humanReadable(humanReadable) {
}

string XmlWriter::toString(const xml_document* doc, bool humanReadable) {
	ostringstream o;
	if (humanReadable) {
		doc->save(o);
	} else {
		doc->save(o, "\t", format_raw);
	}
	return o.str();
}

///////////////////////////////////////////////////////////////////////////////

void XmlWriter::saveDocument(xml_node* parent, const DocumentDTO& document) {
	xml_node node = parent->append_child("irrigation");

	if (document.hasPrograms()) {
		saveProgramList(&node, document.getPrograms());
	}
}

void XmlWriter::saveProgramList(pugi::xml_node* parent, const list<ProgramDTO>& programs) {
	xml_node programListNode = parent->append_child("programs");

	for (auto& program : programs) {
		saveProgram(&programListNode, program);
	}
}

void XmlWriter::saveProgram(xml_node* parent, const ProgramDTO& program) {
	xml_node node = parent->append_child("program");

	if (program.hasId()) {
		node.append_attribute("id").set_value(program.getId());
	}

	if (program.hasName()) {
		node.append_child("name").text().set(program.getName().c_str());
	}

	if (program.hasSchedulerType()) {
		node.append_child("schedulertype").text().set(program.getSchedulerType().c_str());
	}

	if (program.hasPeriodicScheduler() || program.hasWeeklyScheduler()) {
		xml_node schedulersListNode = node.append_child("schedulers");

		if (program.hasPeriodicScheduler()) {
			const PeriodicSchedulerDTO& periodicScheduler = program.getPeriodicScheduler();
			savePeriodicScheduler(&schedulersListNode, periodicScheduler);
		}

		if (program.hasWeeklyScheduler()) {
			const WeeklySchedulerDTO& weeklyScheduler = program.getWeeklyScheduler();
			saveWeeklyScheduler(&schedulersListNode, weeklyScheduler);
		}
	}

	if (program.hasRunTimes()) {
		saveRunTimeList(&node, program.getRunTimes());
	}

	if (program.hasStartTimes()) {
		saveStartTimeList(&node, program.getStartTimes());
	}
}

void XmlWriter::saveRunTimeList(pugi::xml_node* parent, const list<RunTimeDTO>& runTimes) {
	xml_node runTimeListNode = parent->append_child("runtimes");

	for (auto& runTime : runTimes) {
		saveRunTime(&runTimeListNode, runTime);
	}
}

void XmlWriter::saveRunTime(xml_node* parent, const RunTimeDTO& runTime) {
	xml_node node = parent->append_child("runtime");

	if (runTime.hasMinutes()) {
		node.append_child("minute").text().set(runTime.getMinutes());
	}

	if (runTime.hasSeconds()) {
		node.append_child("second").text().set(runTime.getSeconds());
	}

	if (runTime.hasId()) {
		node.append_attribute("id").set_value(runTime.getId());
	}
}

void XmlWriter::saveStartTimeList(pugi::xml_node* parent, const list<StartTimeDTO>& startTimes) {
	xml_node startTimeListNode = parent->append_child("starttimes");

	for (auto& startTime : startTimes) {
		saveStartTime(&startTimeListNode, startTime);
	}
}

void XmlWriter::saveStartTime(xml_node* parent, const StartTimeDTO& startTime) {
	xml_node node = parent->append_child("starttime");

	if (startTime.hasHours()) {
		node.append_child("hour").text().set(startTime.getHours());
	}

	if (startTime.hasMinutes()) {
		node.append_child("minute").text().set(startTime.getMinutes());
	}

	if (startTime.hasId()) {
		node.append_attribute("id").set_value(startTime.getId());
	}
}

void XmlWriter::savePeriodicScheduler(xml_node* parent, const PeriodicSchedulerDTO& scheduler) {
	xml_node node = parent->append_child("scheduler");
	node.append_attribute("type").set_value("periodic");

	if (scheduler.hasAdjustment()) {
		node.append_child("adjustment").text().set(scheduler.getAdjustment());
	}

	if (scheduler.hasValues()) {
		xml_node daysNode = node.append_child("days");
		const list<bool>& values = scheduler.getValues();
		for (auto value : values) {
			daysNode.append_child("day").text().set(value);
		}
	}

	if (scheduler.hasPeriodStartYear() ||
		scheduler.hasPeriodStartMonth() ||
		scheduler.hasPeriodStartDay()) {

		xml_node startDateNode = node.append_child("periodStartDate");
		if (scheduler.hasPeriodStartYear()) {
			startDateNode.append_child("year").text().set(scheduler.getPeriodStartYear());
		}

		if (scheduler.hasPeriodStartMonth()) {
			startDateNode.append_child("month").text().set(scheduler.getPeriodStartMonth());
		}

		if (scheduler.hasPeriodStartDay()) {
			startDateNode.append_child("day").text().set(scheduler.getPeriodStartDay());
		}
	}
}

void XmlWriter::saveWeeklyScheduler(xml_node* parent, const WeeklySchedulerDTO& scheduler) {
	xml_node node = parent->append_child("scheduler");
	node.append_attribute("type").set_value("weekly");

	if (scheduler.hasAdjustment()) {
		node.append_child("adjustment").text().set(scheduler.getAdjustment());
	}

	if (scheduler.hasValues()) {
		xml_node daysNode = node.append_child("days");
		const list<bool>& values = scheduler.getValues();
		for (auto value : values) {
			daysNode.append_child("day").text().set(value);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

string XmlWriter::save(const DocumentDTO& document) {
	unique_ptr<xml_document> doc(new xml_document());
	saveDocument(doc.get(), document);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const ProgramDTO& program) {
	unique_ptr<xml_document> doc(new xml_document());
	saveProgram(doc.get(), program);
	return toString(doc.get(), humanReadable);
}

std::string XmlWriter::save(const list<ProgramDTO>& programs) {
	unique_ptr<xml_document> doc(new xml_document());
	saveProgramList(doc.get(), programs);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const RunTimeDTO& runTime) {
	unique_ptr<xml_document> doc(new xml_document());
	saveRunTime(doc.get(), runTime);
	return toString(doc.get(), humanReadable);
}

std::string XmlWriter::save(const list<RunTimeDTO>& runTimes) {
	unique_ptr<xml_document> doc(new xml_document());
	saveRunTimeList(doc.get(), runTimes);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const StartTimeDTO& startTime) {
	unique_ptr<xml_document> doc(new xml_document());
	saveStartTime(doc.get(), startTime);
	return toString(doc.get(), humanReadable);
}

std::string XmlWriter::save(const list<StartTimeDTO>& startTimes) {
	unique_ptr<xml_document> doc(new xml_document());
	saveStartTimeList(doc.get(), startTimes);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const PeriodicSchedulerDTO& scheduler) {
	unique_ptr<xml_document> doc(new xml_document());
	savePeriodicScheduler(doc.get(), scheduler);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const WeeklySchedulerDTO& scheduler) {
	unique_ptr<xml_document> doc(new xml_document());
	saveWeeklyScheduler(doc.get(), scheduler);
	return toString(doc.get(), humanReadable);
}
