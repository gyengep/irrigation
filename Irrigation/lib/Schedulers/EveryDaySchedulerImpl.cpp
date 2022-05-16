#include "EveryDaySchedulerImpl.h"

///////////////////////////////////////////////////////////////////////////////

Scheduler::Result EveryDaySchedulerImpl::process(const time_t rawtime) {
	return Scheduler::Result(true);
}

EveryDaySchedulerDTO EveryDaySchedulerImpl::toEveryDaySchedulerDto() const {
	return EveryDaySchedulerDTO();
}

void EveryDaySchedulerImpl::updateFromEveryDaySchedulerDto(const EveryDaySchedulerDTO& schedulerDTO) {
}

std::string EveryDaySchedulerImpl::toString() const {
	std::ostringstream oss;
	oss << "EveryDayScheduler{}";
	return oss.str();
}
