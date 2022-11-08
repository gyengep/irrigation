#include "EveryDaySchedulerImpl.h"
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

EveryDaySchedulerPtr EveryDaySchedulerImplFactory::create() const {
	return std::make_shared<EveryDaySchedulerImpl>();
}

///////////////////////////////////////////////////////////////////////////////

Scheduler::Result EveryDaySchedulerImpl::process(const DateTime&) {
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
