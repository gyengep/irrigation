#include "Scheduler.h"
#include <stdexcept>
#include <string>

using namespace std;



string to_string(SchedulerType schedulerType) {
	switch (schedulerType) {
	case SchedulerType::SPECIFIED_DAYS:
		return "specified";
	default:
		throw invalid_argument("Unknown value of enum SchedulerType: " + to_string(static_cast<unsigned>(schedulerType)));
	}
}
