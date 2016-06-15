/*
 * Document.cpp
 *
 *  Created on: 2016. m�j. 15.
 *      Author: Rendszergazda
 */

#include "common.h"
#include <stdexcept>
#include <sstream>
#include "Document.h"
#include "Program.h"
#include "Valve.h"
#include "View.h"

Document::Document() {
	valves.resize(VALVE_COUNT);
	
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
		//std::lock_guard<std::mutex> guard(viewMutex);
		std::list<View*>::iterator itView;
		for (itView = views.begin(); views.end() != itView; ++itView) {
			delete (*itView);
		}
		views.clear();
	}

	if (true) {
		//std::lock_guard<std::mutex> guard(valveMutex);
		std::vector<Valve*>::iterator itValve;
		for (itValve = valves.begin(); valves.end() != itValve; ++itValve) {
			delete (*itValve);
		}
		valves.clear();
	}
}

/////////////////////////////////////////////////////
// Program

const Document::ProgramList& Document::getPrograms() const {
	programMutex.lock_shared();
	return programs;
}

void Document::releasePrograms() const {
	programMutex.unlock_shared();
}

Program& Document::addProgram() {
	std::lock_guard<std::shared_timed_mutex> lock(programMutex);
	programs.push_back(new Program(this));
	return *programs.back();
}

void Document::deleteProgram(unsigned id) {
	std::lock_guard<std::shared_timed_mutex> lock(programMutex);
	ProgramList::const_iterator it = getProgram_notSafe(id);

	if (programs.end() == it) {
		throw std::runtime_error("Program doesn't exist");
	}

	deletedPrograms.push_back(*it);
	programs.erase(it);
}

void Document::moveProgram(unsigned id, unsigned newIdx) {
	std::lock_guard<std::shared_timed_mutex> lock(programMutex);

	if (programs.size() <= newIdx) {
		throw std::out_of_range("Invalid program index");
	}

	ProgramList::const_iterator it = getProgram_notSafe(id);
	if (programs.end() == it) {
		throw std::runtime_error("Program doesn't exist");
	}

	Program* program = *it;
	programs.erase(it);

	unsigned count = 0;
	for (it = programs.begin(); count < newIdx; ++count) {
		++it;
	}

	programs.insert(it, program);
}

Program& Document::getProgram(unsigned id) {
	std::shared_lock<std::shared_timed_mutex> lock(programMutex);

	ProgramList::const_iterator it = getProgram_notSafe(id);

	if (programs.end() == it) {
		throw std::runtime_error("Program doesn't exist");
	}

	return **it;
}

Document::ProgramList::const_iterator Document::getProgram_notSafe(unsigned id) const {
	for (auto it = programs.begin(); programs.end() != it; ++it) {
		if ((*it)->getID() == id) {
			return it;
		}
	}

	return programs.end();
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
// Zone

void Document::openZone(unsigned i, bool open) {
	if (ZONE_COUNT <= i) { 
		throw std::out_of_range("Zone index out of range"); 
	} 
	
	std::lock_guard<std::mutex> guard(valveMutex);
	openValve(i, open);
	openValve(ZONE_COUNT, open);
}

/////////////////////////////////////////////////////
// Valve

void Document::openValve(unsigned i, bool open) { 
	if (valves.size() <= i) { 
		throw std::out_of_range("Valve index out of range"); 
	} 
	
	//std::lock_guard<std::mutex> guard(valveMutex);
	printf("OpenValve(%u, %s)\n", i, open ? "true" : "false");
	//valves[i]->open(open);
}
