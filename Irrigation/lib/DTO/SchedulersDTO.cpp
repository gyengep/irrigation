#include "SchedulersDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


WeeklySchedulerDTO::WeeklySchedulerDTO(const WeeklySchedulerDTO& other) {
	if (other.hasAdjustment()) {
		setAdjustment(other.getAdjustment());
	}

	if (other.hasValues()) {
		setValues(list<bool>(other.getValues()));
	}
}

WeeklySchedulerDTO::WeeklySchedulerDTO(unsigned adjustment, list<bool>&& values) {
	setAdjustment(adjustment);
	setValues(move(values));
}

bool WeeklySchedulerDTO::operator== (const WeeklySchedulerDTO& other) const {
	return (equalsPtr(adjustment.get(), other.adjustment.get()) &&
			equalsPtr(values.get(), other.values.get()));
}

bool WeeklySchedulerDTO::hasAdjustment() const {
	return (adjustment.get() != nullptr);
}

bool WeeklySchedulerDTO::hasValues() const {
	return (values.get() != nullptr);
}

unsigned WeeklySchedulerDTO::getAdjustment() const {
	if (!hasAdjustment()) {
		throw logic_error("WeeklySchedulerDTO::getAdjustment(): !hasAdjustment()");
	}

	return *adjustment.get();
}

const list<bool>& WeeklySchedulerDTO::getValues() const {
	if (!hasValues()) {
		throw logic_error("WeeklySchedulerDTO::getValues(): !hasValues()");
	}

	return *values.get();
}

WeeklySchedulerDTO& WeeklySchedulerDTO::setAdjustment(unsigned adjustment) {
	this->adjustment.reset(new unsigned(adjustment));
	return *this;
}

WeeklySchedulerDTO& WeeklySchedulerDTO::setValues(list<bool>&& values) {
	this->values.reset(new list<bool>(move(values)));
	return *this;
}

ostream& operator<<(ostream& os, const WeeklySchedulerDTO& scheduler) {
	os << "WeeklySchedulerDTO{";
	PRINT_PTR(os, "adjustment", scheduler.adjustment.get());
	os << ", ";
	PRINT_PTR(os, "values", scheduler.values.get());
	os << "}";

	return os;
}
