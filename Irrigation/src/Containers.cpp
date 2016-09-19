/*
 * Containers.cpp
 *
 *  Created on: 2016.09.19.
 *      Author: 502664609
 */

#include "Containers.h"
#include "Program.h"
#include "Tools.h"


////////////////////////////////////////////////////////////////
// RUN TIME

RunTimeContainer::RunTimeContainer() {
	for (IdType i = 0; i < runTimes.size(); i++) {
		runTimes[i] = std::make_pair(i, 0);
	}
}

RunTimeContainer::~RunTimeContainer() {
}

const RunTimeContainer::RunTimes& RunTimeContainer::container() const {
	return runTimes;
}

void RunTimeContainer::set(IdType id, unsigned minutes) {
	try {
		tools::set(runTimes, id, minutes);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_RUNTIMEID);
	}
}

unsigned RunTimeContainer::get(IdType id) const {
	unsigned result;

	try {
		result = tools::get(runTimes, id);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_RUNTIMEID);
	}

	return result;
}

////////////////////////////////////////////////////////////////
// START TIME

StartTimeContainer::StartTimeContainer() : nextStartTimeId(0) {
}

StartTimeContainer::~StartTimeContainer() {
}

const StartTimeContainer::StartTimes& StartTimeContainer::container() const {
	return startTimes;
}

IdType StartTimeContainer::add(unsigned minutes) {
	IdType startTimeId = nextStartTimeId++;
	tools::push_back(startTimes, startTimeId, minutes);
	return startTimeId;
}

void StartTimeContainer::del(IdType id) {
	try {
		tools::erase(startTimes, id);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_STARTTIMEID);
	}
}

void StartTimeContainer::set(IdType id, unsigned minutes) {
	try {
		tools::set(startTimes, id, minutes);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_STARTTIMEID);
	}
}

unsigned StartTimeContainer::get(IdType id) const {
	unsigned result;

	try {
		result = tools::get(startTimes, id);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_STARTTIMEID);
	}

	return result;
}

/////////////////////////////////////////////////////
// Program

ProgramContainer::ProgramContainer() : nextProgramId(0) {
}

ProgramContainer::~ProgramContainer() {

	std::lock_guard<std::mutex> lock(mutex);

	for (auto it = programs.begin(); programs.end() != it; ++it) {
		Program* program = it->second;
		delete program;
	}

	programs.clear();
}

const ProgramContainer::Programs& ProgramContainer::container() const {
	if (mutex.try_lock()) {
		throw not_locked("Programs are not locked");
	}

	return programs;
}

Program& ProgramContainer::add() {
	if (mutex.try_lock()) {
		throw not_locked("Programs are not locked");
	}

	Program* program = new Program();
	tools::push_back(programs, nextProgramId, program);
	nextProgramId++;
	return *program;
}

void ProgramContainer::del(IdType id) {
	if (mutex.try_lock()) {
		throw not_locked("Programs are not locked");
	}

	Program* program = NULL;

	try {
		program = tools::erase(programs, id);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_PROGRAMID);
	}

	delete program;
}

void ProgramContainer::move(IdType id, unsigned newPosition) {
	if (mutex.try_lock()) {
		throw not_locked("Programs are not locked");
	}

	if (programs.size() <= newPosition) {
		throw std::out_of_range("Invalid position");
	}

	Program* program;

	try {
		program = tools::erase(programs, id);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_PROGRAMID);
	}

	unsigned count = 0;
	auto it = programs.begin();
	while (count < newPosition) {
		++it;
		++count;
	}

	programs.insert(it, std::make_pair(id, program));
}

Program& ProgramContainer::get(IdType id) {
	if (mutex.try_lock()) {
		throw not_locked("Programs are not locked");
	}

	Program* program = NULL;
	try {
		program = tools::get(programs, id);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_PROGRAMID);
	}

	return *program;
}

const Program& ProgramContainer::get(IdType id) const {
	if (mutex.try_lock()) {
		throw not_locked("Programs are not locked");
	}

	const Program* program = NULL;
	try {
		program = tools::get(programs, id);
	} catch(invalid_id& e) {
		throw invalid_id(INVALID_PROGRAMID);
	}

	return *program;
}


