#include "RunTimeContainer.h"
#include "RunTime.h"
#include <memory>
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Hardware/ZoneConfig.h"
#include "Logger/Logger.h"

using namespace std;


RunTime* RunTimeFactory::createRunTime() const {
	return new RunTime();
}

///////////////////////////////////////////////////////////////////////////////

RunTimeContainer::RunTimeContainer() {
	unique_ptr<const RunTimeFactory> ptr(new RunTimeFactory());
	init(move(ptr));
}

RunTimeContainer::RunTimeContainer(const RunTimeFactory* runTimeFactory) {
	unique_ptr<const RunTimeFactory> ptr(runTimeFactory);
	init(move(ptr));
}

RunTimeContainer::~RunTimeContainer() {
	for (auto it = container.begin(); it != container.end(); ++it) {
		delete it->second;
	}
}

void RunTimeContainer::init(unique_ptr<const RunTimeFactory> runTimeFactory) {
	container.reserve(ZONE_COUNT);
	for (size_t i = 0; i < ZONE_COUNT; i++) {
		container.push_back(make_pair(i, runTimeFactory->createRunTime()));
	}
}

const RunTimeContainer::mapped_type& RunTimeContainer::at(const key_type& key) const {
	if (container.size() <= key) {
		throw NoSuchElementException("RunTime with id " + to_string(key) + " not found");
	}

	return container[key].second;
}

RunTimeContainer::mapped_type& RunTimeContainer::at(const key_type& key) {
	if (container.size() <= key) {
		throw NoSuchElementException("RunTime with id " + to_string(key) + " not found");
	}

	return container[key].second;
}

string to_string(const RunTimeContainer& runTimeContainer) {
	ostringstream o;
	o << "[";
	for (auto it = runTimeContainer.begin(); it != runTimeContainer.end(); ++it) {
		if (it != runTimeContainer.begin()) {
			o << ", ";
		}
		o << "{" << to_string(it->first) << ", " << to_string(*it->second) << "}";
	}
	o << "]";
	return o.str();
}
