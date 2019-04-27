#include "PeriodicSchedulerDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


PeriodicSchedulerDTO::PeriodicSchedulerDTO(const PeriodicSchedulerDTO& other) {
	if (other.hasValues()) {
		setValues(list<bool>(other.getValues()));
	}

	if (other.hasPeriodStartYear()) {
		setPeriodStartYear(other.getPeriodStartYear());
	}

	if (other.hasPeriodStartMonth()) {
		setPeriodStartMonth(other.getPeriodStartMonth());
	}

	if (other.hasPeriodStartDay()) {
		setPeriodStartDay(other.getPeriodStartDay());
	}
}

PeriodicSchedulerDTO::PeriodicSchedulerDTO(list<bool>&& values,
		unsigned periodStartYear, unsigned periodStartMonth, unsigned periodStartDay) {
	setValues(move(values));
	setPeriodStartYear(periodStartYear);
	setPeriodStartMonth(periodStartMonth);
	setPeriodStartDay(periodStartDay);
}

bool PeriodicSchedulerDTO::operator== (const PeriodicSchedulerDTO& other) const {
	return (equalsPtr(values.get(), other.values.get()) &&
			equalsPtr(periodStartYear.get(), other.periodStartYear.get()) &&
			equalsPtr(periodStartMonth.get(), other.periodStartMonth.get()) &&
			equalsPtr(periodStartDay.get(), other.periodStartDay.get()));
}

bool PeriodicSchedulerDTO::hasValues() const {
	return (values.get() != nullptr);
}

bool PeriodicSchedulerDTO::hasPeriodStartYear() const {
	return (periodStartYear.get() != nullptr);
}

bool PeriodicSchedulerDTO::hasPeriodStartMonth() const {
	return (periodStartMonth.get() != nullptr);
}

bool PeriodicSchedulerDTO::hasPeriodStartDay() const {
	return (periodStartDay.get() != nullptr);
}

const list<bool>& PeriodicSchedulerDTO::getValues() const {
	if (!hasValues()) {
		throw logic_error("PeriodicSchedulerDTO::getValues(): !hasValues()");
	}

	return *values.get();
}

unsigned PeriodicSchedulerDTO::getPeriodStartYear() const {
	if (!hasPeriodStartYear()) {
		throw logic_error("PeriodicSchedulerDTO::getPeriodStartYear(): !hasPeriodStartYear()");
	}

	return *periodStartYear.get();
}

unsigned PeriodicSchedulerDTO::getPeriodStartMonth() const {
	if (!hasPeriodStartMonth()) {
		throw logic_error("PeriodicSchedulerDTO::getPeriodStartMont(): !PeriodStartMont()");
	}

	return *periodStartMonth.get();
}

unsigned PeriodicSchedulerDTO::getPeriodStartDay() const {
	if (!hasPeriodStartDay()) {
		throw logic_error("PeriodicSchedulerDTO::getPeriodStartYear(): !hasPeriodStartDay()");
	}

	return *periodStartDay.get();
}

PeriodicSchedulerDTO& PeriodicSchedulerDTO::setValues(list<bool>&& values) {
	this->values.reset(new list<bool>(move(values)));
	return *this;
}

PeriodicSchedulerDTO& PeriodicSchedulerDTO::setPeriodStartYear(unsigned periodStartYear) {
	this->periodStartYear.reset(new unsigned(periodStartYear));
	return *this;
}

PeriodicSchedulerDTO& PeriodicSchedulerDTO::setPeriodStartMonth(unsigned periodStartMonth) {
	this->periodStartMonth.reset(new unsigned(periodStartMonth));
	return *this;
}

PeriodicSchedulerDTO& PeriodicSchedulerDTO::setPeriodStartDay(unsigned periodStartDay) {
	this->periodStartDay.reset(new unsigned(periodStartDay));
	return *this;
}

ostream& operator<<(ostream& os, const PeriodicSchedulerDTO& scheduler) {
	os << "PeriodicSchedulerDTO{";
	PRINT_PTR(os, "values", scheduler.values.get());
	os << ", ";
	PRINT_PTR(os, "periodStartYear", scheduler.periodStartYear.get());
	os << ", ";
	PRINT_PTR(os, "periodStartMonth", scheduler.periodStartMonth.get());
	os << ", ";
	PRINT_PTR(os, "periodStartDay", scheduler.periodStartDay.get());
	os << "}";

	return os;
}
