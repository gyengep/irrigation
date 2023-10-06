#include "EveryDaySchedulerImpl.h"
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

EveryDaySchedulerPtr EveryDaySchedulerImplFactory::create() const {
	return std::make_shared<EveryDaySchedulerImpl>();
}

///////////////////////////////////////////////////////////////////////////////

void EveryDaySchedulerImpl::createPersistedData() {
}

void EveryDaySchedulerImpl::deletePersistedData() {
}

void EveryDaySchedulerImpl::loadPersistedData() {
}

std::unique_ptr<Scheduler::Result> EveryDaySchedulerImpl::process(const LocalDateTime&) {
	return std::unique_ptr<Scheduler::Result>(new Scheduler::Result(true));
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
