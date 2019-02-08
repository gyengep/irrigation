#include "PeriodicScheduler.h"
#include <ctime>
#include <iomanip>
#include <mutex>
#include <time.h>
#include "Exceptions/Exceptions.h"
#include "Utils/ToString.h"
#include "Utils/TimeConversion.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

PeriodicScheduler::PeriodicScheduler() : PeriodicScheduler(100, vector<bool>(), 1970, 1, 1) {
}

PeriodicScheduler::PeriodicScheduler(const PeriodicScheduler& other) :
	adjustment(other.adjustment),
	days(other.days),
	periodStartYear(other.periodStartYear),
	periodStartMonth(other.periodStartMonth),
	periodStartDay(other.periodStartDay),
	elapsedDaysSinceEpochToPeriodStart(other.elapsedDaysSinceEpochToPeriodStart)
{
}

PeriodicScheduler::PeriodicScheduler(unsigned adjustment, const vector<bool>& days, unsigned year, unsigned month, unsigned day) :
	adjustment(adjustment)
{
	setPeriod(days.size());
	this->days = days;

	setPeriodStartDate(year, month, day);
}

PeriodicScheduler::~PeriodicScheduler() {
}

bool PeriodicScheduler::operator== (const PeriodicScheduler& other) const {
	return (adjustment == other.adjustment &&
			days == other.days &&
			periodStartYear == other.periodStartYear &&
			periodStartMonth == other.periodStartMonth &&
			periodStartDay == other.periodStartDay);
}

void PeriodicScheduler::setPeriod(unsigned days) {
	if (days > maxPeriod) {
		throw ValueOutOfBoundsException(
				"Period value shall be less than " + to_string(maxPeriod) +
				", while actual value is " + to_string(days));
	}

	this->days.resize(days, false);
}

unsigned PeriodicScheduler::getPeriod() const {
	return days.size();
}

void PeriodicScheduler::checkIndex(size_t day) const {
	if (days.size() <= day) {
		throw IndexOutOfBoundsException(
				"Day index shall be less than " + to_string(days.size()) +
				", while actual value is " + to_string(day));
	}
}

void PeriodicScheduler::enableDay(size_t day, bool enable) {
	checkIndex(day);
	days[day] = enable;
}

bool PeriodicScheduler::isDayEnabled(size_t day) const {
	checkIndex(day);
	return days[day];
}

void PeriodicScheduler::setAdjustment(unsigned adjustment) {
	this->adjustment = adjustment;
}

unsigned PeriodicScheduler::getAdjustment() const {
	return adjustment;
}

void PeriodicScheduler::setPeriodStartDate(unsigned year, unsigned month, unsigned day) {
	const tm timeinfo = toCalendarTime(year, month, day);
	elapsedDaysSinceEpochToPeriodStart = getElapsedDaysSinceEpoch(timeinfo);

	periodStartYear = year;
	periodStartMonth = month;
	periodStartDay = day;
}

bool PeriodicScheduler::isDayScheduled(const tm& timeinfo) const {
	unsigned const period = getPeriod();

	if (0 == period) {
		return false;
	}

	unsigned offset = elapsedDaysSinceEpochToPeriodStart % period;
	unsigned index = (getElapsedDaysSinceEpoch(timeinfo) + period - offset) % period;
	return isDayEnabled(index);
}

PeriodicSchedulerDTO PeriodicScheduler::toPeriodicSchedulerDto() const {
	return PeriodicSchedulerDTO(adjustment, list<bool>(days.begin(), days.end()),
			periodStartYear, periodStartMonth, periodStartDay);
}

void PeriodicScheduler::updateFromPeriodicSchedulerDto(const PeriodicSchedulerDTO& schedulerDTO) {
	if (schedulerDTO.hasAdjustment()) {
		setAdjustment(schedulerDTO.getAdjustment());
	}

	if (schedulerDTO.hasValues()) {
		setPeriod(schedulerDTO.getValues().size());
		copy(schedulerDTO.getValues().begin(), schedulerDTO.getValues().end(), days.begin());
	}

	if (schedulerDTO.hasPeriodStartYear() ||
		schedulerDTO.hasPeriodStartMonth() ||
		schedulerDTO.hasPeriodStartDay()) {

		if (!schedulerDTO.hasPeriodStartYear() ||
			!schedulerDTO.hasPeriodStartMonth() ||
			!schedulerDTO.hasPeriodStartDay()) {
			throw runtime_error("PeriodicScheduler::updateFromDTO(): both of the "
					"startYear, startMonth and startDay have to be exist");
		}

		setPeriodStartDate(
			schedulerDTO.getPeriodStartYear(),
			schedulerDTO.getPeriodStartMonth(),
			schedulerDTO.getPeriodStartDay());
	}
}


string to_string(const PeriodicScheduler& periodicScheduler) {
	ostringstream oss;
	oss << periodicScheduler;
	return oss.str();
}

ostream& operator<<(ostream& os, const PeriodicScheduler& periodicScheduler) {
	os << "PeriodicScheduler{";
	os << "adjustment=" << periodicScheduler.getAdjustment() << "%, ";
	os << "values=" << to_string(periodicScheduler.days.begin(), periodicScheduler.days.end()) << ", ";
	os << "periodStartDate=" <<
			setw(4) << setfill('0') << to_string(periodicScheduler.periodStartYear) << "-" <<
			setw(2) << setfill('0') << to_string(periodicScheduler.periodStartMonth) << "-" <<
			setw(2) << setfill('0') << to_string(periodicScheduler.periodStartDay);
	os << "}";
	return os;
}
