/*
 * Document.cpp
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "Document.h"
#include "Program.h"
#include "Valve.h"
#include "View.h"

#include <stdexcept>
#include <sstream>


Document::Document() : nextProgramID(0) {
	valves[0] = new Valve(0);
	valves[1] = new Valve(2);
	valves[2] = new Valve(3);
	valves[3] = new Valve(4);
	valves[4] = new Valve(5);
	valves[5] = new Valve(6);
	valves[6] = new Valve(27);
}

Document::~Document() {
	if (true) {
		std::lock_guard<std::mutex> guard(viewMutex);
		for (auto it = views.begin(); views.end() != it; ++it) {
			delete (*it);
		}
		views.clear();
	}

	if (true) {
		std::lock_guard<std::mutex> guard(valveMutex);
		for (auto it = valves.begin(); valves.end() != it; ++it) {
			delete (*it);
		}
	}
}

/////////////////////////////////////////////////////
// Program

const Document::ProgramList& Document::getPrograms() const {
	programMutex.lock();
	return programs;
}

void Document::releasePrograms() const {
	programMutex.unlock();
}

Program& Document::addProgram() {
	std::lock_guard<std::mutex> lock(programMutex);

	Program* program = new Program(this);
	tools::push_back(programs, nextProgramID, program);
	nextProgramID++;
	return *program;
}

void Document::deleteProgram(IdType id) {
	std::lock_guard<std::mutex> lock(programMutex);

	Program* program;
	try {
		program = tools::erase(programs, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_PROGRAMID);
	}

	deletedPrograms.push_back(program);
}

void Document::moveProgram(IdType id, unsigned newPos) {
/*
	std::lock_guard<std::mutex> lock(programMutex);

	if (programs.size() <= newIdx) {
		throw std::out_of_range("Invalid program index");
	}

	ProgramList::const_iterator it = getProgram_notSafe(id);
	if (programs.end() == it) {
		throw std::runtime_error(INVALID_PROGRAMID);
	}

	Program* program = *it;
	programs.erase(it);

	unsigned count = 0;
	for (it = programs.begin(); count < newIdx; ++count) {
		++it;
	}

	programs.insert(it, program);
	*/
}

Program& Document::getProgram(IdType id) {
	std::lock_guard<std::mutex> lock(programMutex);

	Program* program = NULL;
	try {
		program = tools::get(programs, id);
	} catch(not_found_exception& e) {
		throw not_found_exception(INVALID_PROGRAMID);
	}

	return *program;
}


/////////////////////////////////////////////////////
// View

void Document::addView(View* view) {
	std::lock_guard<std::mutex> guard(viewMutex);
	views.push_back(view);
}

void Document::updateViews() {
	std::lock_guard<std::mutex> guard(viewMutex);
	for (auto it = views.begin(); views.end() != it; ++it) {
		(*it)->update();
	}
}

/////////////////////////////////////////////////////
// Zone, Valve

void Document::openZone(IdType id, bool open) {
	if (getZoneCount() <= id) {
		throw std::out_of_range("Invalid zoneID");
	} 
	
	std::lock_guard<std::mutex> guard(valveMutex);
	openValve(id, open);
	openValve(getZoneCount(), open);
}

void Document::openValve(IdType id, bool open) {
	if (getValveCount() <= id) {
		throw std::out_of_range("Invalid valveID");
	} 
	
	std::lock_guard<std::mutex> guard(valveMutex);
#ifdef __arm__
	valves[i]->open(open);
#else
	printf("OpenValve(%lu, %s)\n", id, open ? "true" : "false");
#endif // __arm__

}
