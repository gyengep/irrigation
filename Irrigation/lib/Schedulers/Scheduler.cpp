#include "Scheduler.h"
#include <stdexcept>
#include <string>

using namespace std;



string to_string(SchedulerType schedulerType) {
	switch (schedulerType) {
	case SchedulerType::WEEKLY:
		return "weekly";
	case SchedulerType::PERIODIC:
		return "periodic";
	case SchedulerType::EVERY_DAY:
		return "every_day";
	case SchedulerType::FIXED_AMOUNT:
		return "fixed_amount";
	case SchedulerType::FIXED_PERIOD:
		return "fixed_period";
	default:
		throw invalid_argument("Unknown value of enum SchedulerType: " + to_string(static_cast<unsigned>(schedulerType)));
	}
}

Scheduler::Result::Result(bool isScheduled) :
	isScheduled(isScheduled),
	overrideAdjustment(false),
	adjustment(0)
{
}

Scheduler::Result::Result(unsigned adjustment) :
	isScheduled(adjustment > 0),
	overrideAdjustment(true),
	adjustment(adjustment)
{
}

Scheduler::Result::Result(bool isScheduled, bool overrideAdjustment, unsigned adjustment) :
	isScheduled(isScheduled),
	overrideAdjustment(overrideAdjustment),
	adjustment(adjustment)
{
}

bool Scheduler::Result::operator== (const Result& other) const {
	return  (isScheduled == other.isScheduled) &&
			(overrideAdjustment == other.overrideAdjustment) &&
			(adjustment == other.adjustment);
}
