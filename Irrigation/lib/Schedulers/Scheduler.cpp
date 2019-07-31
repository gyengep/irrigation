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
