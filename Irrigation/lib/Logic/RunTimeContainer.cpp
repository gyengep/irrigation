#include "RunTimeContainer.h"
#include "RunTime.h"
#include <memory>
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"
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

RunTimeContainer::RunTimeContainer(const RunTimeContainer& other) :
	RunTimeContainer()
{
	 this->operator =(other);
}

RunTimeContainer::RunTimeContainer(initializer_list<unsigned> initializer) :
	RunTimeContainer()
{
	if (container.size() != initializer.size()) {
		throw logic_error("container.size() != initializer.size()");
	}

	for (size_t i = 0; i < container.size(); ++i) {
		RunTime& target = *next(container.begin(), i)->second;
		unsigned value = *next(initializer.begin(), i);
		target = RunTime(value);
	}
}

RunTimeContainer::RunTimeContainer(RunTimeFactory* runTimeFactory) {
	unique_ptr<RunTimeFactory> runTimeFactoryPtr(runTimeFactory);

	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); i++) {
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

RunTimeContainer& RunTimeContainer::operator= (const RunTimeContainer& other) {
	if (this != &other) {
		for (size_t i = 0; i < container.size(); ++i) {
			RunTime& target = *at(i);
			const RunTime& source = *other.at(i);
			target = source;
		}
	}

	return *this;
}

bool RunTimeContainer::operator== (const RunTimeContainer& other) const {
	for (size_t i = 0; i < container.size(); ++i) {
		const RunTime& runTime1 = *at(i);
		const RunTime& runTime2 = *other.at(i);

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
