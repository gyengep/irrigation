#include "Impl/WeeklySchedulerImpl.h"
#include "Exceptions/Exceptions.h"
#include "Utils/ToString.h"
#include <algorithm>
#include <list>
#include <sstream>
#include <stdexcept>

///////////////////////////////////////////////////////////////////////////////

WeeklySchedulerPtr WeeklySchedulerImplFactory::create() const {
	return std::make_shared<WeeklySchedulerImpl>();
}

///////////////////////////////////////////////////////////////////////////////

WeeklySchedulerImpl::WeeklySchedulerImpl() :
	days(size, false)
{
}

WeeklySchedulerImpl::WeeklySchedulerImpl(std::initializer_list<bool> initializer) :
	days(initializer)
{
	if (size != initializer.size()) {
		throw std::logic_error("WeeklySchedulerImpl() initializer.size() has to be 7");
	}

	for (size_t i = 0; i < days.size(); ++i) {
		days[i] = *std::next(initializer.begin(), i);
	}
}

WeeklySchedulerImpl::~WeeklySchedulerImpl() {
}

void WeeklySchedulerImpl::checkIndex(size_t day) const {
	if (days.size() <= day) {
		throw IndexOutOfBoundsException(
				"Day index shall be less than " + std::to_string(days.size()) +
				", while actual value is " + std::to_string(day));
	}
}

void WeeklySchedulerImpl::enableDay(size_t day, bool enable) {
	checkIndex(day);
	days[day] = enable;
}

bool WeeklySchedulerImpl::isDayEnabled(size_t day) const {
	checkIndex(day);
	return days[day];
}

std::unique_ptr<Scheduler::Result> WeeklySchedulerImpl::process(const LocalDateTime& localDateTime) {
	const size_t weekDay = localDateTime.getDayOfWeek();
	checkIndex(weekDay);
	return std::unique_ptr<Scheduler::Result>(new Scheduler::Result(days[weekDay]));
}

WeeklySchedulerDto WeeklySchedulerImpl::toWeeklySchedulerDto() const {
	return WeeklySchedulerDto(std::list<bool>(days.begin(), days.end()));
}

void WeeklySchedulerImpl::updateFromWeeklySchedulerDto(const WeeklySchedulerDto& schedulerDto) {
	if (schedulerDto.hasValues()) {
		if (schedulerDto.getValues().size() != days.size()) {
			throw IllegalArgumentException("WeeklySchedulerImpl::updateFromWeeklySchedulerDto(): " + std::to_string(days.size()) +
					"\"days\" have to be exist");
		}

		std::copy(schedulerDto.getValues().begin(), schedulerDto.getValues().end(), days.begin());
	}
}

std::string WeeklySchedulerImpl::toString() const {
	std::ostringstream oss;
	oss << "WeeklyScheduler{";
	oss << "values=" << to_string(days.begin(), days.end());
	oss << "}";
	return oss.str();
}
