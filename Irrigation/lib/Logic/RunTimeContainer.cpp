#include "RunTimeContainer.h"
#include "RunTime.h"
#include <memory>
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"

using namespace std;


shared_ptr<RunTime> RunTimeFactory::createRunTime() const {
	return shared_ptr<RunTime>(new RunTime());
}

///////////////////////////////////////////////////////////////////////////////

RunTimeContainer::RunTimeContainer() :
	RunTimeContainer(unique_ptr<RunTimeFactory>(new RunTimeFactory()))
{
}

RunTimeContainer::RunTimeContainer(unique_ptr<RunTimeFactory> runTimeFactory) {
	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); i++) {
		container.push_back(make_pair(i, runTimeFactory->createRunTime()));
	}
}

RunTimeContainer::RunTimeContainer(const RunTimeContainer& other) {
	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); i++) {
		const RunTime& otherRunTime = *other.container[i].second;
		container.push_back(make_pair(i, shared_ptr<RunTime>(new RunTime(otherRunTime))));
	}
}

RunTimeContainer::RunTimeContainer(initializer_list<RunTime> initializer) {
	if (ZoneHandler::getZoneCount() != initializer.size()) {
		throw logic_error("ZoneHandler::getZoneCount() != initializer.size()");
	}

	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); i++) {
		const RunTime& otherRunTime = *next(initializer.begin(), i);
		container.push_back(make_pair(i, shared_ptr<RunTime>(new RunTime(otherRunTime))));
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

void RunTimeContainer::updateFromRunTimeDtoList(const std::list<RunTimeDTO>& runTimeDtoList) {
	if (runTimeDtoList.size() != container.size()) {
		throw IllegalArgumentException(
				"RunTimeDTO list must contains " + to_string(size()) + " elements");
	}

	for (size_t i = 0; i < container.size(); ++i) {
		const RunTimeDTO& runTimeDto =*next(runTimeDtoList.begin(), i);
		if (runTimeDto.hasId()) {
			if (runTimeDto.getId() != i) {
				throw IllegalArgumentException(
					"The id attribute of the RunTimeDtoLists " + to_string(i) + "st/nd/rd item has to be " + to_string(i));
			}
		}

		at(i)->updateFromRunTimeDto(runTimeDto);
	}
}

string to_string(const RunTimeContainer& runTimeContainer) {
	ostringstream oss;
	oss << runTimeContainer;
	return oss.str();
}

std::ostream& operator<<(std::ostream& os, const RunTimeContainer& runTimeContainer) {
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
