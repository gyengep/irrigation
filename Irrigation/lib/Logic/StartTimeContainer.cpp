#include "StartTimeContainer.h"
#include "StartTime.h"
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"

using namespace std;


StartTimeContainer::container_type::const_iterator StartTimeContainer::find(const key_type& key) const {
	for (auto it = container.begin(); it != container.end(); ++it) {
		if (it->first == key) {
			return it;
		}
	}

	throw NoSuchElementException("StartTime[" + to_string(key) + "] not found");
}

const StartTimeContainer::mapped_type& StartTimeContainer::at(const key_type& key) const {
	return find(key)->second;
}

StartTimeContainer::value_type& StartTimeContainer::insert(const key_type& key, mapped_type&& value) {
	for (auto& startTimeAndIdPair : container) {
		if (startTimeAndIdPair.first == key) {
			throw AlreadyExistException("StartTime[" + to_string(key) + "] is already exist");
		}
	}

	container.push_back(make_pair(key, move(value)));
	LOGGER.debug("StartTime[%s] added: %s",
		to_string(key).c_str(),
		to_string(*container.back().second.get()).c_str()
		);
	return container.back();
}

void StartTimeContainer::erase(const key_type& key) {
	container.erase(find(key));
	LOGGER.debug("StartTime[%s] deleted", to_string(key).c_str());
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
