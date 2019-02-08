#include "WeeklyScheduler.h"
#include <algorithm>
#include <ctime>
#include <list>
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Utils/ToString.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

WeeklyScheduler::WeeklyScheduler() : WeeklyScheduler(100, array<bool, 7>({false, false, false, false, false, false, false})) {
}

WeeklyScheduler::WeeklyScheduler(const WeeklyScheduler& other) : WeeklyScheduler(other.adjustment, other.days) {
}

WeeklyScheduler::WeeklyScheduler(unsigned adjustment, const array<bool, 7>& days) :
	adjustment(adjustment),
	days(days)
{
}

WeeklyScheduler::~WeeklyScheduler() {
}

bool WeeklyScheduler::operator== (const WeeklyScheduler& other) const {
	return (adjustment == other.adjustment &&
			days == other.days);
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

WeeklySchedulerDTO WeeklyScheduler::toWeeklySchedulerDto() const {
	return WeeklySchedulerDTO(adjustment, list<bool>(days.begin(), days.end()));
}

void WeeklyScheduler::updateFromWeeklySchedulerDto(const WeeklySchedulerDTO& schedulerDTO) {
	if (schedulerDTO.hasAdjustment()) {
		setAdjustment(schedulerDTO.getAdjustment());
	}

	if (schedulerDTO.hasValues()) {
		if (schedulerDTO.getValues().size() != days.size()) {
			throw runtime_error("WeeklyScheduler::updateFromDTO(): " + to_string(days.size()) +
					"\"days\" have to be exist");
		}

		copy(schedulerDTO.getValues().begin(), schedulerDTO.getValues().end(), days.begin());
	}
}

string to_string(const WeeklyScheduler& weeklyScheduler) {
	ostringstream oss;
	oss << weeklyScheduler;
	return oss.str();
}

ostream& operator<<(ostream& os, const WeeklyScheduler& weeklyScheduler) {
	os << "WeeklyScheduler{";
	os << "adjustment=" << weeklyScheduler.getAdjustment() << "%, ";
	os << "values=" << to_string(weeklyScheduler.days.begin(), weeklyScheduler.days.end());
	os << "}";
	return os;
}
