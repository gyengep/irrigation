#include "RunTimeContainer.h"
#include <memory>
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

RunTimeContainer::RunTimeContainer(const std::shared_ptr<RunTimeFactory>& runTimeFactory) {
	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); i++) {
		container.emplace_back(make_pair(IdType(i), runTimeFactory->create()));
	}
}

RunTimeContainer::RunTimeContainer(initializer_list<RunTimePtr> initializer) {
	if (ZoneHandler::getZoneCount() != initializer.size()) {
		throw logic_error("ZoneHandler::getZoneCount() != initializer.size()");
	}

	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); ++i) {
		container.emplace_back(make_pair(IdType(i), *next(initializer.begin(), i)));
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

RunTimeContainer::const_mapped_type RunTimeContainer::at(const key_type& key) const {
	if (container.size() <= key) {
		throw NoSuchElementException("RunTime[" + to_string(key) + "] does not exist");
	}

	return container[key].second;
}

RunTimeContainer::mapped_type RunTimeContainer::at(const key_type& key) {
	if (container.size() <= key) {
		throw NoSuchElementException("RunTime[" + to_string(key) + "] does not exist");
	}

	return container[key].second;
}

list<RunTimeDTO> RunTimeContainer::toRunTimeDtoList() const {
	list<RunTimeDTO> runTimeDtoList;
	for (size_t i = 0; i < container.size(); ++i) {
		runTimeDtoList.emplace_back(container[i].second->toRunTimeDto().setId(container[i].first));
	}
	return runTimeDtoList;
}

void RunTimeContainer::updateFromRunTimeDtoList(const list<RunTimeDTO>& runTimeDtoList) {
	if (runTimeDtoList.size() != container.size()) {
		throw IllegalArgumentException(
				"RunTimeDtoList has to contain " + to_string(size()) + " elements");
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
