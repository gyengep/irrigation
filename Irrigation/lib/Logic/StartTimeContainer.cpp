#include "StartTimeContainer.h"
#include "Exceptions.h"
#include "StartTime.h"
#include "Logger/Logger.h"

using namespace std;


StartTimeContainer::StartTimeContainer() {
}

StartTimeContainer::~StartTimeContainer() {
	for (auto it = container.begin(); it != container.end(); ++it) {
		delete it->second;
	}
}

StartTimeContainer::container_type::iterator StartTimeContainer::find(const key_type& key) {
	for (auto it = container.begin(); it != container.end(); ++it) {
		if (it->first == key) {
			return it;
		}
	}

	throw InvalidStartTimeIdException(key);
}

StartTimeContainer::container_type::const_iterator StartTimeContainer::find(const key_type& key) const {
	for (auto it = container.begin(); it != container.end(); ++it) {
		if (it->first == key) {
			return it;
		}
	}

	throw InvalidStartTimeIdException(key);
}

const StartTimeContainer::mapped_type& StartTimeContainer::at(const key_type& key) const {
	return find(key)->second;
}

StartTimeContainer::mapped_type& StartTimeContainer::at(const key_type& key) {
	return find(key)->second;
}

const StartTimeContainer::mapped_type& StartTimeContainer::insert(const key_type& key, const mapped_type& value) {

	for (auto it = container.begin(); it != container.end(); ++it) {
		if (it->first == key) {
			delete value;
			throw StartTimeIdExist(key);
		}
	}

	container.push_back(make_pair(key, value));
	LOGGER.info("StartTime %u added", key);
	return value;
}

void StartTimeContainer::erase(const key_type& key) {
	auto it = find(key);
	delete it->second;
	container.erase(it);
	LOGGER.info("StartTime %u deleted", key);
}

void StartTimeContainer::sort() {
	container.sort(compareStartTime);
}

bool StartTimeContainer::compareStartTime(const value_type& first, const value_type& second) {
	StartTime& firstStartTime = *first.second;
	StartTime& secondStartTime = *second.second;

	return (firstStartTime < secondStartTime);
}

