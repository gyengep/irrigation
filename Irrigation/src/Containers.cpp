/*
 * Containers.cpp
 *
 *  Created on: 2016.09.19.
 *      Author: 502664609
 */

#include "Containers.h"
#include "Tools.h"

Containers::Containers() {
	// TODO Auto-generated constructor stub

}

Containers::~Containers() {
	// TODO Auto-generated destructor stub
}

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
