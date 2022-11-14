#include "Scheduler.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;



string to_string(SchedulerType schedulerType) {
	switch (schedulerType) {
	case SchedulerType::EVERY_DAY:
		return "every-day";
	case SchedulerType::HOT_WEATHER:
		return "hot-weather";
	case SchedulerType::TEMPERATURE_DEPENDENT:
		return "temperature-dependent";
	case SchedulerType::WEEKLY:
		return "weekly";
	default:
		throw invalid_argument("Unknown value of enum SchedulerType: " + to_string(static_cast<unsigned>(schedulerType)));
	}
}

Scheduler::Result::Result(bool scheduled) :
	adjustment(scheduled ? 100 : 0)
{
}

Scheduler::Result::Result(unsigned adjustment) :
	adjustment(adjustment)
{
}

bool Scheduler::Result::isSkipped() const {
	return (0 == adjustment);
}

unsigned Scheduler::Result::getAdjustment() const {
	return adjustment;
}

bool Scheduler::Result::operator== (const Result& other) const {
	return  (adjustment == other.adjustment);
}

ostream& operator<<(ostream& os, const Scheduler::Result& result) {
	ostringstream oss;
	os << "SchedulerResult{";
	os << "adjustment: " << result.getAdjustment() << "%";
	os << "}";
	return os;
}
