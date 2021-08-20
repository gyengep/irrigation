#include "EveryDayScheduler.h"
#include <ctime>
#include <iomanip>
#include <mutex>
#include <time.h>
#include "Exceptions/Exceptions.h"
#include "Utils/ToString.h"
#include "Utils/TimeConversion.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

bool EveryDayScheduler::operator== (const EveryDayScheduler& other) const {
	return true;
}

Scheduler::Result EveryDayScheduler::process(const time_t rawtime) {
	return Scheduler::Result(true);
}

EveryDaySchedulerDTO EveryDayScheduler::toEveryDaySchedulerDto() const {
	return EveryDaySchedulerDTO();
}

void EveryDayScheduler::updateFromEveryDaySchedulerDto(const EveryDaySchedulerDTO& schedulerDTO) {
}

string to_string(const EveryDayScheduler& scheduler) {
	ostringstream oss;
	oss << scheduler;
	return oss.str();
}

ostream& operator<<(ostream& os, const EveryDayScheduler& scheduler) {
	os << "EveryDayScheduler{}";
	return os;
}
