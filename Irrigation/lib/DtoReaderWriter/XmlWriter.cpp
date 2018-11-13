#include "XmlWriter.h"
#include <sstream>
#include "pugixml.hpp"
#include "DTO/DocumentDTO.h"
#include "DTO/ProgramDTO.h"
#include "DTO/RunTimeDTO.h"
#include "DTO/StartTimeDTO.h"
#include "DTO/SchedulersDTO.h"

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
		xml_node programListNode = node.append_child("programs");
		const list<ProgramDTO>& programs = document.getPrograms();
		for (auto& program : programs) {
			saveProgram(&programListNode, program);
		}
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

	if (program.hasWeeklyScheduler()) {
		const WeeklySchedulerDTO& scheduler = program.getWeeklyScheduler();
		xml_node schedulersListNode = node.append_child("schedulers");
		saveScheduler(&schedulersListNode, scheduler);
	}

	if (program.hasRunTimes()) {
		const list<RunTimeDTO>& runTimes = program.getRunTimes();
		xml_node runTimeListNode = node.append_child("runtimes");
		for (auto& runTime : runTimes) {
			saveRunTime(&runTimeListNode, runTime);
		}
	}

	if (program.hasStartTimes()) {
		const list<StartTimeDTO>& startTimes = program.getStartTimes();
		xml_node startTimeListNode = node.append_child("starttimes");
		for (auto& startTime : startTimes) {
			saveStartTime(&startTimeListNode, startTime);
		}
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

void XmlWriter::saveStartTime(xml_node* parent, const StartTimeDTO& startTime) {
	xml_node node = parent->append_child("starttime");

	if (startTime.hasHour()) {
		node.append_child("hour").text().set(startTime.getHour());
	}

	if (startTime.hasMinute()) {
		node.append_child("minute").text().set(startTime.getMinute());
	}

	if (startTime.hasId()) {
		node.append_attribute("id").set_value(startTime.getId());
	}
}

void XmlWriter::saveScheduler(xml_node* parent, const WeeklySchedulerDTO& scheduler) {
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

string XmlWriter::save(const RunTimeDTO& runTime) {
	unique_ptr<xml_document> doc(new xml_document());
	saveRunTime(doc.get(), runTime);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const StartTimeDTO& startTime) {
	unique_ptr<xml_document> doc(new xml_document());
	saveStartTime(doc.get(), startTime);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const WeeklySchedulerDTO& scheduler) {
	unique_ptr<xml_document> doc(new xml_document());
	saveScheduler(doc.get(), scheduler);
	return toString(doc.get(), humanReadable);
}
