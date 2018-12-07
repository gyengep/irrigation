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

unique_ptr<PeriodicScheduler> SchedulerFactory::createPeriodicScheduler() const {
	return unique_ptr<PeriodicScheduler>(new PeriodicScheduler());
}

///////////////////////////////////////////////////////////////////////////////

PeriodicScheduler::PeriodicScheduler() :
	adjustment(100),
	periodStartYear(1970),
	periodStartMonth(1),
	periodStartDay(1)
{
	const tm timeinfo = toCalendarTime(periodStartYear, periodStartMonth, periodStartDay);
	elapsedDaysSinceEpochToPeriodStart = getElapsedDaysSinceEpoch(timeinfo);
}

PeriodicScheduler::~PeriodicScheduler() {
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

PeriodicSchedulerDTO PeriodicScheduler::getPeriodicSchedulerDTO() const {
	return PeriodicSchedulerDTO(adjustment, list<bool>(days.begin(), days.end()),
			periodStartYear, periodStartMonth, periodStartDay);
}

void PeriodicScheduler::updateFromDTO(const PeriodicSchedulerDTO& schedulerDTO) {
	if (schedulerDTO.hasAdjustment()) {
		setAdjustment(schedulerDTO.getAdjustment());
	}

	if (schedulerDTO.hasValues()) {
		setPeriod(schedulerDTO.getValues().size());
		for (size_t i = 0; i < schedulerDTO.getValues().size(); ++i) {
			auto it = next(schedulerDTO.getValues().begin(), i);
			days[i] = *it;
		}
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

		setPeriodStartDate(schedulerDTO.getPeriodStartYear(),
				schedulerDTO.getPeriodStartMonth(),
				schedulerDTO.getPeriodStartDay());
	}
}

string to_string(const PeriodicScheduler& periodicScheduler) {
	ostringstream o;
	o << "PeriodicScheduler{";
	o << "adjustment=" << periodicScheduler.getAdjustment() << "%, ";
	o << "values=" << to_string(periodicScheduler.days.begin(), periodicScheduler.days.end()) << ", ";
	o << "periodStartDate=" <<
			setw(4) << setfill('0') << to_string(periodicScheduler.periodStartYear) << "-" <<
			setw(2) << setfill('0') << to_string(periodicScheduler.periodStartMonth) << "-" <<
			setw(2) << setfill('0') << to_string(periodicScheduler.periodStartDay);
	o << "}";
	return o.str();
}
