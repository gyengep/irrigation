#include "WeeklyScheduler.h"
#include <ctime>
#include <list>
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Utils/ToString.h"

using namespace std;



unique_ptr<WeeklyScheduler> SchedulerFactory::createWeeklyScheduler() const {
	return unique_ptr<WeeklyScheduler>(new WeeklyScheduler());
}

///////////////////////////////////////////////////////////////////////////////

WeeklyScheduler::WeeklyScheduler() :
	days(DAY_COUNT, false),
	adjustment(100)
{
}

WeeklyScheduler::~WeeklyScheduler() {
}

void WeeklyScheduler::checkIndex(size_t day) const {
	if (days.size() <= day) {
		throw IndexOutOfBoundsException(
				"Day index shall be less than " + to_string(days.size()) +
				", while actual value is " + to_string(day));
	}
}

void WeeklyScheduler::setAdjustment(unsigned adjustment) {
	this->adjustment = adjustment;
}

unsigned WeeklyScheduler::getAdjustment() const {
	return adjustment;
}

void WeeklyScheduler::enableDay(size_t day, bool enable) {
	checkIndex(day);
	days[day] = enable;
}

bool WeeklyScheduler::isDayEnabled(size_t day) const {
	checkIndex(day);
	return days[day];
}

bool WeeklyScheduler::isDayScheduled(const tm& timeinfo) const {
	const size_t weekDay = timeinfo.tm_wday;
	checkIndex(weekDay);
	return days[(weekDay + 6) % 7];
}

WeeklySchedulerDTO WeeklyScheduler::getWeeklySchedulerDTO() const {
	return WeeklySchedulerDTO(adjustment, list<bool>(days.begin(), days.end()));
}

void WeeklyScheduler::updateFromDTO(const WeeklySchedulerDTO& schedulerDTO) {
	if (schedulerDTO.hasAdjustment()) {
		setAdjustment(schedulerDTO.getAdjustment());
	}

	if (schedulerDTO.hasValues()) {
		const size_t maxIndex = min(days.size(), schedulerDTO.getValues().size());
		for (size_t i = 0; i < maxIndex; ++i) {
			auto it = next(schedulerDTO.getValues().begin(), i);
			days[i] = *it;
		}
	}
}

string to_string(const WeeklyScheduler& weeklyScheduler) {
	ostringstream o;
	o << "WeeklyScheduler{";
	o << "adjustment=" << weeklyScheduler.getAdjustment() << "% ";
	o << "values=" << to_string(weeklyScheduler.days.begin(), weeklyScheduler.days.end());
	o << "}";
	return o.str();
}
