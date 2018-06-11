#include "SchedulersDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


SpecifiedSchedulerDTO::SpecifiedSchedulerDTO() {
}

SpecifiedSchedulerDTO::SpecifiedSchedulerDTO(const SpecifiedSchedulerDTO& other) {
	if (other.hasValues()) {
		setValues(new list<bool>(other.getValues()));
	}
}

SpecifiedSchedulerDTO::SpecifiedSchedulerDTO(const list<bool>* values) {
	setValues(values);
}

bool SpecifiedSchedulerDTO::operator== (const SpecifiedSchedulerDTO& other) const {
	return (equalsPtr(values.get(), other.values.get()));
}

bool SpecifiedSchedulerDTO::hasValues() const {
	return (values.get() != nullptr);
}

const list<bool>& SpecifiedSchedulerDTO::getValues() const {
	if (!hasValues()) {
		throw logic_error("SpecifiedSchedulerDTO::getValues()");
	}

	return *values.get();
}

SpecifiedSchedulerDTO& SpecifiedSchedulerDTO::setValues(const list<bool>* values) {
	this->values.reset(values);
	return *this;
}

ostream& operator<<(ostream& os, const SpecifiedSchedulerDTO& scheduler) {
	os << "SpecifiedSchedulerDTO{";
	PRINT_PTR(os, "values", scheduler.values.get());
	os << "}";

	return os;
}
