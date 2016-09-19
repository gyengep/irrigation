/*
 * Containers.cpp
 *
 *  Created on: 2016.09.19.
 *      Author: 502664609
 */

#include "Containers.h"
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
