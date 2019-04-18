#include "StartTimeContainer.h"
#include "StartTime.h"
#include <algorithm>
#include <sstream>
#include "Exceptions/Exceptions.h"

using namespace std;


StartTimeContainer::StartTimeContainer(const StartTimeContainer& other) {
	for (const auto& value : other.container) {
		container.push_back(make_pair(value.first, shared_ptr<StartTime>(new StartTime(*value.second))));
	}
}

StartTimeContainer::StartTimeContainer(initializer_list<StartTimeContainer::value_type> initializer) {
	for (const auto& value : initializer) {
		insert(value.first, value.second);
	}
}

bool StartTimeContainer::operator== (const StartTimeContainer& other) const {
	if (container.size() != other.container.size()) {
		return false;
	}

	auto comp = [](const value_type& a, const value_type& b) {
		return (a.first == b.first) && (a.second->operator ==(*b.second));
	};

	return equal(container.begin(), container.end(), other.container.begin(), comp);
}

const StartTimeContainer::mapped_type& StartTimeContainer::at(const key_type& key) const {
	auto it = find_if(container.begin(), container.end(), findKey(key));

	if (container.end() == it) {
		throw NoSuchElementException("StartTime[" + to_string(key) + "] does not exist");
	}

	return it->second;
}

void StartTimeContainer::erase(const key_type& key) {
	auto it = find_if(container.begin(), container.end(), findKey(key));

	if (container.end() == it) {
		throw NoSuchElementException("StartTime[" + to_string(key) + "] does not exist");
	}

	container.erase(it);
}

StartTimeContainer::value_type& StartTimeContainer::insert(const key_type& key, const mapped_type& value) {
	if (container.end() != find_if(container.begin(), container.end(), findKey(key))) {
		throw AlreadyExistException("StartTime[" + to_string(key) + "] already exists");
	}

	container.push_back(make_pair(key, value));
	return container.back();
}

void StartTimeContainer::sort() {
	auto comp = [](const value_type& a, const value_type& b) {
		return a.second->operator <(*b.second);
	};

	container.sort(comp);
}

list<StartTimeDTO> StartTimeContainer::toStartTimeDtoList() const {
	list<StartTimeDTO> startTimeDtoList;
	for (const value_type& value : container) {
		startTimeDtoList.push_back(value.second->toStartTimeDto().setId(value.first));
	}
	return startTimeDtoList;
}

void StartTimeContainer::updateFromStartTimeDtoList(const list<StartTimeDTO>& startTimeDtoList) {
	container.clear();
	for (const StartTimeDTO& startTimeDTO : startTimeDtoList) {
		unique_ptr<IdType> id;
		if (startTimeDTO.hasId()) {
			id.reset(new IdType(startTimeDTO.getId()));
		} else {
			id.reset(new IdType());
		}

		shared_ptr<StartTime> startTime(new StartTime());
		startTime->updateFromStartTimeDto(startTimeDTO);
		insert(IdType(*id), startTime);
	}
}

string to_string(const StartTimeContainer& startTimeContainer) {
	ostringstream oss;
	oss << startTimeContainer;
	return oss.str();
}

ostream& operator<<(ostream& os, const StartTimeContainer& startTimeContainer) {
	os << "[";
	for (auto it = startTimeContainer.begin(); it != startTimeContainer.end(); ++it) {
		if (it != startTimeContainer.begin()) {
			os << ", ";
		}
		os << "{" << to_string(it->first) << ", " << to_string(*it->second) << "}";
	}
	os << "]";
	return os;
}
