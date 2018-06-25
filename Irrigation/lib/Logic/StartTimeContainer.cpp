#include "StartTimeContainer.h"
#include "StartTime.h"
#include <sstream>
#include "Exceptions/Exceptions.h"
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

	throw NoSuchElementException("StartTime with id " + to_string(key) + " not found");
}

StartTimeContainer::container_type::const_iterator StartTimeContainer::find(const key_type& key) const {
	for (auto it = container.begin(); it != container.end(); ++it) {
		if (it->first == key) {
			return it;
		}
	}

	throw NoSuchElementException("StartTime with id " + to_string(key) + " not found");
}

const StartTimeContainer::mapped_type& StartTimeContainer::at(const key_type& key) const {
	return find(key)->second;
}

StartTimeContainer::mapped_type& StartTimeContainer::at(const key_type& key) {
	return find(key)->second;
}

StartTimeContainer::value_type& StartTimeContainer::insert(const key_type& key, const mapped_type& value) {

	for (auto it = container.begin(); it != container.end(); ++it) {
		if (it->first == key) {
			delete value;
			throw AlreadyExistException("Program with id " + to_string(key) + " is already exist");
		}
	}

	container.push_back(make_pair(key, value));
	return container.back();
}

void StartTimeContainer::erase(const key_type& key) {
	auto it = find(key);
	delete it->second;
	container.erase(it);
}

void StartTimeContainer::sort() {
	container.sort(compareStartTime);
}

bool StartTimeContainer::compareStartTime(const value_type& first, const value_type& second) {
	StartTime& firstStartTime = *first.second;
	StartTime& secondStartTime = *second.second;

	return (firstStartTime < secondStartTime);
}

string to_string(const StartTimeContainer& startTimeContainer) {
	ostringstream o;
	o << "[";
	for (auto it = startTimeContainer.begin(); it != startTimeContainer.end(); ++it) {
		if (it != startTimeContainer.begin()) {
			o << ", ";
		}
		o << "{" << to_string(it->first) << ", " << to_string(*it->second) << "}";
	}
	o << "]";
	return o.str();
}
