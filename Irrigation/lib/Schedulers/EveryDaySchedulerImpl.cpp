#include "Impl/EveryDaySchedulerImpl.h"
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

EveryDaySchedulerPtr EveryDaySchedulerImplFactory::create() const {
	return std::make_shared<EveryDaySchedulerImpl>();
}

///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Scheduler::Result> EveryDaySchedulerImpl::process(const LocalDateTime&) {
	return std::unique_ptr<Scheduler::Result>(new Scheduler::Result(true));
}

EveryDaySchedulerDto EveryDaySchedulerImpl::toEveryDaySchedulerDto() const {
	return EveryDaySchedulerDto();
}

void EveryDaySchedulerImpl::updateFromEveryDaySchedulerDto(const EveryDaySchedulerDto& schedulerDto) {
}

std::string EveryDaySchedulerImpl::toString() const {
	std::ostringstream oss;
	oss << "EveryDayScheduler{}";
	return oss.str();
}
