#include "RunTimeContainer.h"
#include "RunTime.h"
#include <memory>
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"
#include "Logger/Logger.h"

using namespace std;


shared_ptr<RunTime> RunTimeFactory::createRunTime() const {
	return shared_ptr<RunTime>(new RunTime());
}

///////////////////////////////////////////////////////////////////////////////

RunTimeContainer::RunTimeContainer() :
	RunTimeContainer(unique_ptr<RunTimeFactory>(new RunTimeFactory()))
{
}

RunTimeContainer::RunTimeContainer(const RunTimeContainer& other) :
	RunTimeContainer()
{
	 this->operator =(other);
}

RunTimeContainer::RunTimeContainer(initializer_list<RunTime> initializer) :
	RunTimeContainer()
{
	if (container.size() != initializer.size()) {
		throw logic_error("container.size() != initializer.size()");
	}

	for (size_t i = 0; i < container.size(); ++i) {
		RunTime& target = *next(container.begin(), i)->second;
		const RunTime& source = *next(initializer.begin(), i);
		target = source;
	}
}

RunTimeContainer::RunTimeContainer(unique_ptr<RunTimeFactory> runTimeFactory) {
	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); i++) {
		container.push_back(make_pair(i, runTimeFactory->createRunTime()));
	}
}

const RunTimeContainer::mapped_type& RunTimeContainer::at(const key_type& key) const {
	if (container.size() <= key) {
		throw NoSuchElementException("RunTime[" + to_string(key) + "] not found");
	}

	return container[key].second;
}

RunTimeContainer& RunTimeContainer::operator= (const RunTimeContainer& other) {
	if (this != &other) {
		for (size_t i = 0; i < container.size(); ++i) {
			RunTime& target = *container.at(i).second;
			const RunTime& source = *other.container.at(i).second;
			target = source;
		}
	}

	return *this;
}

bool RunTimeContainer::operator== (const RunTimeContainer& other) const {
	for (size_t i = 0; i < container.size(); ++i) {
		const RunTime& runTime1 = *container.at(i).second;
		const RunTime& runTime2 = *other.container.at(i).second;

		if (!(runTime1 == runTime2)) {
			return false;
		}
	}

	return true;
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
