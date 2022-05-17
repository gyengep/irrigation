#include "WeeklySchedulerImpl.h"
#include <algorithm>
#include <ctime>
#include <list>
#include <sstream>
#include <stdexcept>
#include "Exceptions/Exceptions.h"
#include "Utils/TimeConversion.h"
#include "Utils/ToString.h"

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

bool WeeklySchedulerImpl::onProcess(const time_t rawtime) const {
	const size_t weekDay = toLocalTime(rawtime).tm_wday;
	checkIndex(weekDay);
	return days[(weekDay + 6) % 7];
}

Scheduler::Result WeeklySchedulerImpl::process(const std::time_t rawtime) {
	return Scheduler::Result(onProcess(rawtime));
}

WeeklySchedulerDTO WeeklySchedulerImpl::toWeeklySchedulerDto() const {
	return WeeklySchedulerDTO(std::list<bool>(days.begin(), days.end()));
}

void WeeklySchedulerImpl::updateFromWeeklySchedulerDto(const WeeklySchedulerDTO& schedulerDTO) {
	if (schedulerDTO.hasValues()) {
		if (schedulerDTO.getValues().size() != days.size()) {
			throw std::runtime_error("WeeklySchedulerImpl::updateFromDTO(): " + std::to_string(days.size()) +
					"\"days\" have to be exist");
		}

		std::copy(schedulerDTO.getValues().begin(), schedulerDTO.getValues().end(), days.begin());
	}
}

std::string WeeklySchedulerImpl::toString() const {
	std::ostringstream oss;
	oss << "WeeklyScheduler{";
	oss << "values=" << to_string(days.begin(), days.end());
	oss << "}";
	return oss.str();
}
