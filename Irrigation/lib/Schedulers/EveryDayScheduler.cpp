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

EveryDayScheduler::EveryDayScheduler() {
}

EveryDayScheduler::EveryDayScheduler(const EveryDayScheduler& other) {
}

EveryDayScheduler::~EveryDayScheduler() {
}

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

string to_string(const EveryDayScheduler& periodicScheduler) {
	ostringstream oss;
	oss << periodicScheduler;
	return oss.str();
}

ostream& operator<<(ostream& os, const EveryDayScheduler& periodicScheduler) {
	os << "EveryDayScheduler{}";
	return os;
}
