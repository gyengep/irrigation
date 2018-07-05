#include "SchedulersDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


SpecifiedSchedulerDTO::SpecifiedSchedulerDTO() {
}

SpecifiedSchedulerDTO::SpecifiedSchedulerDTO(const SpecifiedSchedulerDTO& other) {
	if (other.hasAdjustment()) {
		setAdjustment(other.getAdjustment());
	}

	if (other.hasValues()) {
		setValues(new list<bool>(other.getValues()));
	}
}

SpecifiedSchedulerDTO::SpecifiedSchedulerDTO(unsigned adjustment, const list<bool>* values) {
	setAdjustment(adjustment);
	setValues(values);
}

bool SpecifiedSchedulerDTO::operator== (const SpecifiedSchedulerDTO& other) const {
	return (equalsPtr(adjustment.get(), other.adjustment.get()) &&
			equalsPtr(values.get(), other.values.get()));
}

bool SpecifiedSchedulerDTO::hasAdjustment() const {
	return (adjustment.get() != nullptr);
}

bool SpecifiedSchedulerDTO::hasValues() const {
	return (values.get() != nullptr);
}

unsigned SpecifiedSchedulerDTO::getAdjustment() const {
	if (!hasAdjustment()) {
		throw logic_error("SpecifiedSchedulerDTO::getAdjustment(): !hasAdjustment()");
	}

	return *adjustment.get();
}

const list<bool>& SpecifiedSchedulerDTO::getValues() const {
	if (!hasValues()) {
		throw logic_error("SpecifiedSchedulerDTO::getValues(): !hasValues()");
	}

	return *values.get();
}

SpecifiedSchedulerDTO& SpecifiedSchedulerDTO::setAdjustment(unsigned adjustment) {
	this->adjustment.reset(new unsigned(adjustment));
	return *this;
}

SpecifiedSchedulerDTO& SpecifiedSchedulerDTO::setValues(const list<bool>* values) {
	this->values.reset(values);
	return *this;
}

ostream& operator<<(ostream& os, const SpecifiedSchedulerDTO& scheduler) {
	os << "SpecifiedSchedulerDTO{";
	PRINT_PTR(os, "adjustment", scheduler.adjustment.get());
	os << ", ";
	PRINT_PTR(os, "values", scheduler.values.get());
	os << "}";

	return os;
}
