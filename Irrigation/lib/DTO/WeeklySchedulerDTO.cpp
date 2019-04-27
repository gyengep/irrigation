#include "WeeklySchedulerDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


WeeklySchedulerDTO::WeeklySchedulerDTO(const WeeklySchedulerDTO& other) {
	if (other.hasValues()) {
		setValues(list<bool>(other.getValues()));
	}
}

WeeklySchedulerDTO::WeeklySchedulerDTO(list<bool>&& values) {
	setValues(move(values));
}

bool WeeklySchedulerDTO::operator== (const WeeklySchedulerDTO& other) const {
	return (equalsPtr(values.get(), other.values.get()));
}

bool WeeklySchedulerDTO::hasValues() const {
	return (values.get() != nullptr);
}

const list<bool>& WeeklySchedulerDTO::getValues() const {
	if (!hasValues()) {
		throw logic_error("WeeklySchedulerDTO::getValues(): !hasValues()");
	}

	return *values.get();
}

WeeklySchedulerDTO& WeeklySchedulerDTO::setValues(list<bool>&& values) {
	this->values.reset(new list<bool>(move(values)));
	return *this;
}

ostream& operator<<(ostream& os, const WeeklySchedulerDTO& scheduler) {
	os << "WeeklySchedulerDTO{";
	PRINT_PTR(os, "values", scheduler.values.get());
	os << "}";

	return os;
}
