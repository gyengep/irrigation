#include "XmlWriter.h"
#include <sstream>
#include "pugixml.hpp"
#include "DTO/DocumentDTO.h"
#include "DTO/ProgramDTO.h"
#include "DTO/RunTimeDTO.h"
#include "DTO/StartTimeDTO.h"
#include "DTO/SchedulersDTO.h"

using namespace std;


XmlWriter::XmlWriter() {
}

XmlWriter::~XmlWriter() {
}

string XmlWriter::toString(const pugi::xml_document* doc, bool humanReadable) {
	ostringstream o;
	if (humanReadable) {
		doc->save(o);
	} else {
		doc->save(o, "\t", pugi::format_raw);
	}
	return o.str();
}

///////////////////////////////////////////////////////////////////////////////

void XmlWriter::saveDocument(pugi::xml_node* parent, const DocumentDTO& document) {
	pugi::xml_node node = parent->append_child("irrigation");

	if (document.hasPrograms()) {
		pugi::xml_node programListNode = node.append_child("programs");
		const list<ProgramDTO>& programs = document.getPrograms();
		for (auto it = programs.begin(); it != programs.end(); ++it) {
			saveProgram(&programListNode, *it);
		}
	}
}

void XmlWriter::saveProgram(pugi::xml_node* parent, const ProgramDTO& program) {
	pugi::xml_node node = parent->append_child("program");

	if (program.hasId()) {
		node.append_attribute("id").set_value(program.getId());
	}

	if (program.hasName()) {
		node.append_child("name").text().set(program.getName().c_str());
	}

	if (program.hasSchedulerType()) {
		node.append_child("schedulertype").text().set(program.getSchedulerType().c_str());
	}

	if (program.hasSpecifiedScheduler()) {
		const SpecifiedSchedulerDTO& scheduler = program.getSpecifiedScheduler();
		pugi::xml_node schedulersListNode = node.append_child("schedulers");
		saveScheduler(&schedulersListNode, scheduler);
	}

	if (program.hasRunTimes()) {
		const list<RunTimeDTO>& runTimes = program.getRunTimes();
		pugi::xml_node runTimeListNode = node.append_child("runtimes");
		for (auto it = runTimes.begin(); it != runTimes.end(); ++it) {
			saveRunTime(&runTimeListNode, *it);
		}
	}

	if (program.hasStartTimes()) {
		const list<StartTimeDTO>& startTimes = program.getStartTimes();
		pugi::xml_node startTimeListNode = node.append_child("starttimes");
		for (auto it = startTimes.begin(); it != startTimes.end(); ++it) {
			saveStartTime(&startTimeListNode, *it);
		}
	}
}

void XmlWriter::saveRunTime(pugi::xml_node* parent, const RunTimeDTO& runTime) {
	pugi::xml_node node = parent->append_child("runtime");

	if (runTime.hasValue()) {
		node.text().set(runTime.getValue());
	}

	if (runTime.hasId()) {
		node.append_attribute("id").set_value(runTime.getId());
	}
}

void XmlWriter::saveStartTime(pugi::xml_node* parent, const StartTimeDTO& startTime) {
	pugi::xml_node node = parent->append_child("starttime");

	if (startTime.hasValue()) {
		node.text().set(startTime.getValue());
	}

	if (startTime.hasId()) {
		node.append_attribute("id").set_value(startTime.getId());
	}
}

void XmlWriter::saveScheduler(pugi::xml_node* parent, const SpecifiedSchedulerDTO& scheduler) {
	pugi::xml_node node = parent->append_child("scheduler");
	node.append_attribute("type").set_value("specified");

	if (scheduler.hasValues()) {
		const list<bool>& values = scheduler.getValues();
		for (auto it = values.begin(); it != values.end(); ++it) {
			node.append_child("day").text().set(*it);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

string XmlWriter::save(const DocumentDTO& document, bool humanReadable) {
	unique_ptr<pugi::xml_document> doc(new pugi::xml_document());
	saveDocument(doc.get(), document);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const ProgramDTO& program, bool humanReadable) {
	unique_ptr<pugi::xml_document> doc(new pugi::xml_document());
	saveProgram(doc.get(), program);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const RunTimeDTO& runTime, bool humanReadable) {
	unique_ptr<pugi::xml_document> doc(new pugi::xml_document());
	saveRunTime(doc.get(), runTime);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const StartTimeDTO& startTime, bool humanReadable) {
	unique_ptr<pugi::xml_document> doc(new pugi::xml_document());
	saveStartTime(doc.get(), startTime);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const SpecifiedSchedulerDTO& scheduler, bool humanReadable) {
	unique_ptr<pugi::xml_document> doc(new pugi::xml_document());
	saveScheduler(doc.get(), scheduler);
	return toString(doc.get(), humanReadable);
}
