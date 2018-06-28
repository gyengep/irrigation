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

RunTimeContainer::RunTimeContainer() :
	RunTimeContainer(new RunTimeFactory())
{
}


RunTimeContainer::RunTimeContainer(RunTimeFactory* runTimeFactory) {
	unique_ptr<RunTimeFactory> runTimeFactoryPtr(runTimeFactory);

	container.reserve(ZONE_COUNT);
	for (size_t i = 0; i < ZONE_COUNT; i++) {

		unique_ptr<RunTime> runTime(runTimeFactoryPtr->createRunTime());
		container.push_back(make_pair(i, move(runTime)));
	}
}

RunTimeContainer::~RunTimeContainer() {
}

const RunTimeContainer::mapped_type::element_type* RunTimeContainer::at(const key_type& key) const {
	if (container.size() <= key) {
		throw NoSuchElementException("RunTime with id " + to_string(key) + " not found");
	}

	return container[key].second.get();
}

RunTimeContainer::mapped_type::element_type* RunTimeContainer::at(const key_type& key) {
	if (container.size() <= key) {
		throw NoSuchElementException("RunTime with id " + to_string(key) + " not found");
	}

	return container[key].second.get();
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
