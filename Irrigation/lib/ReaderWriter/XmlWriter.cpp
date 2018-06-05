#include "XmlWriter.h"
#include <sstream>
#include "pugixml.hpp"
#include "DocumentDTO.h"
#include "ProgramDTO.h"
#include "RunTimeDTO.h"
#include "StartTimeDTO.h"
#include "SchedulersDTO.h"

using namespace std;


XmlWriter::XmlWriter() : doc(new pugi::xml_document()) {
}

XmlWriter::~XmlWriter() {
}

string XmlWriter::toString(bool humanReadable) const {
	ostringstream o;
	if (humanReadable) {
		doc->save(o);
	} else {
		doc->save(o, "\t", pugi::format_raw);
	}
	return o.str();
}

///////////////////////////////////////////////////////////////////////////////

void XmlWriter::saveDocument(pugi::xml_node& parent, const DocumentDTO& document) {
	pugi::xml_node node = parent.append_child("irrigation");

	if (document.getPrograms() != nullptr) {
		pugi::xml_node programListNode = node.append_child("programs");
		const list<ProgramDTO>* programs = document.getPrograms();
		for (auto it = programs->begin(); it != programs->end(); ++it) {
			saveProgram(programListNode, *it);
		}
	}
}

void XmlWriter::saveProgram(pugi::xml_node& parent, const ProgramDTO& program) {
	pugi::xml_node node = parent.append_child("program");

	if (program.getName() != nullptr) {
		node.append_child("name").text().set(program.getName());
	}

	if (program.getSchedulerType() != nullptr) {
		node.append_child("schedulertype").text().set(program.getSchedulerType());
	}

	if (program.getSpecifiedScheduler() != nullptr) {
		const SpecifiedSchedulerDTO& scheduler = *program.getSpecifiedScheduler();
		pugi::xml_node schedulersListNode = node.append_child("schedulers");
		saveScheduler(schedulersListNode, scheduler);
	}

	if (program.getRunTimes() != nullptr) {
		const list<RunTimeDTO>& runTimes = *program.getRunTimes();
		pugi::xml_node runTimeListNode = node.append_child("runtimes");
		for (auto it = runTimes.begin(); it != runTimes.end(); ++it) {
			saveRunTime(runTimeListNode, *it);
		}
	}

	if (program.getStartTimes() != nullptr) {
		const list<StartTimeDTO>& startTimes = *program.getStartTimes();
		pugi::xml_node startTimeListNode = node.append_child("starttimes");
		for (auto it = startTimes.begin(); it != startTimes.end(); ++it) {
			saveStartTime(startTimeListNode, *it);
		}
	}
}

void XmlWriter::saveRunTime(pugi::xml_node& parent, const RunTimeDTO& runTime) {
	if (runTime.getValue() != nullptr) {
		parent.append_child("runtime").text().set(runTime.getValue());
	}
}

void XmlWriter::saveStartTime(pugi::xml_node& parent, const StartTimeDTO& startTime) {
	if (startTime.getValue() != nullptr) {
		parent.append_child("starttime").text().set(startTime.getValue());
	}
}

void XmlWriter::saveScheduler(pugi::xml_node& parent, const SpecifiedSchedulerDTO& scheduler) {
	if (scheduler.getValues() != nullptr) {
		pugi::xml_node node = parent.append_child("scheduler");
		node.append_attribute("type").set_value("specified");

		const list<string>* values = scheduler.getValues();
		for (auto it = values->begin(); it != values->end(); ++it) {
			node.append_child("day").text().set(it->c_str());
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void XmlWriter::save(const DocumentDTO& document) {
	saveDocument(*doc.get(), document);
}

void XmlWriter::save(const ProgramDTO& program) {
	saveProgram(*doc.get(), program);
}

void XmlWriter::save(const RunTimeDTO& runTime) {
	saveRunTime(*doc.get(), runTime);
}

void XmlWriter::save(const StartTimeDTO& startTime) {
	saveStartTime(*doc.get(), startTime);
}

void XmlWriter::save(const SpecifiedSchedulerDTO& scheduler) {
	saveScheduler(*doc.get(), scheduler);
}
