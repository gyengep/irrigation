#include "Scheduler.h"
#include <stdexcept>

using namespace std;



string schedulerTypeToString(SchedulerType schedulerType) {
	switch (schedulerType) {
	case SchedulerType::SPECIFIED_DAYS:
		return "specified";
	default:
		throw invalid_argument("Invalid schedulerType");
	}
}
