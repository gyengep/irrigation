#include "RunTimeContainer.h"
#include "RunTime.h"
#include <memory>
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"

using namespace std;


shared_ptr<RunTime> RunTimeFactory::createRunTime() {
	return shared_ptr<RunTime>(new RunTime());
}

///////////////////////////////////////////////////////////////////////////////

RunTimeContainer::RunTimeContainer() {
	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); i++) {
		shared_ptr<RunTime> runTime(new RunTime());
		container.push_back(make_pair(i, runTime));
	}
}

RunTimeContainer::RunTimeContainer(shared_ptr<RunTimeFactory> runTimeFactory) {
	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); i++) {
		shared_ptr<RunTime> runTime = runTimeFactory->createRunTime();
		container.push_back(make_pair(i, runTime));
	}
}

RunTimeContainer::RunTimeContainer(const RunTimeContainer& other) {
	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); i++) {
		shared_ptr<RunTime> runTime(new RunTime(*other.container[i].second));
		container.push_back(make_pair(i, runTime));
	}
}

RunTimeContainer::RunTimeContainer(initializer_list<RunTime> initializer) {
	if (ZoneHandler::getZoneCount() != initializer.size()) {
		throw logic_error("ZoneHandler::getZoneCount() != initializer.size()");
	}

	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); ++i) {
		shared_ptr<RunTime> runTime(new RunTime(*next(initializer.begin(), i)));
		container.push_back(make_pair(i, runTime));
	}
}

bool RunTimeContainer::operator== (const RunTimeContainer& other) const {
	if (container.size() != other.container.size()) {
		return false;
	}

	auto comp = [](const value_type& a, const value_type& b) {
		return (a.first == b.first) && (a.second->operator ==(*b.second));
	};

	return equal(container.begin(), container.end(), other.container.begin(), comp);
}

const RunTimeContainer::mapped_type& RunTimeContainer::at(const key_type& key) const {
	if (container.size() <= key) {
		throw NoSuchElementException("RunTime[" + to_string(key) + "] not found");
	}

	return container[key].second;
}

list<RunTimeDTO> RunTimeContainer::toRunTimeDtoList() const {
	list<RunTimeDTO> runTimeDtoList;
	for (size_t i = 0; i < container.size(); ++i) {
		runTimeDtoList.push_back(container[i].second->toRunTimeDto().setId(container[i].first));
	}
	return runTimeDtoList;
}

void RunTimeContainer::updateFromRunTimeDtoList(const list<RunTimeDTO>& runTimeDtoList) {
	if (runTimeDtoList.size() != container.size()) {
		throw IllegalArgumentException(
				"RunTimeDTO list must contains " + to_string(size()) + " elements");
	}

	auto it = runTimeDtoList.begin();
	for (size_t i = 0; i < container.size(); ++i, ++it) {
		const RunTimeDTO& runTimeDto = *it;

		if (runTimeDto.hasId()) {
			if (runTimeDto.getId() != i) {
				throw IllegalArgumentException(
					"The id attribute of the RunTimeDtoLists " + to_string(i) + "st/nd/rd item has to be " + to_string(i));
			}
		}

		container[i].second->updateFromRunTimeDto(runTimeDto);
	}
}

string to_string(const RunTimeContainer& runTimeContainer) {
	ostringstream oss;
	oss << runTimeContainer;
	return oss.str();
}

ostream& operator<<(ostream& os, const RunTimeContainer& runTimeContainer) {
	os << "[";
	for (auto it = runTimeContainer.begin(); it != runTimeContainer.end(); ++it) {
		if (it != runTimeContainer.begin()) {
			os << ", ";
		}
		os << "{" << to_string(it->first) << ", " << to_string(*it->second) << "}";
	}
	os << "]";
	return os;
}
