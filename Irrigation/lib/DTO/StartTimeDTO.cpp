#include "StartTimeDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


StartTimeDTO::StartTimeDTO() {
}

StartTimeDTO::StartTimeDTO(const StartTimeDTO& other) {
	if (other.hasId()) {
		setId(other.getId());
	}

	if (other.hasHour()) {
		setHour(other.getHour());
	}

	if (other.hasMinute()) {
		setMinute(other.getMinute());
	}
}

StartTimeDTO::StartTimeDTO(unsigned hour, unsigned minute) {
	setHour(hour);
	setMinute(minute);
}

bool StartTimeDTO::operator== (const StartTimeDTO& other) const {
	return (equalsPtr(id.get(), other.id.get()) &&
			equalsPtr(hour.get(), other.hour.get()) &&
			equalsPtr(minute.get(), other.minute.get()));
}

bool StartTimeDTO::hasId() const {
	return (id.get() != nullptr);
}

bool StartTimeDTO::hasHour() const {
	return (hour.get() != nullptr);
}

bool StartTimeDTO::hasMinute() const {
	return (minute.get() != nullptr);
}

unsigned StartTimeDTO::getId() const {
	if (!hasId()) {
		throw logic_error("StartTimeDTO::getId(): !hasId()");
	}

	return *id.get();
}

unsigned StartTimeDTO::getHour() const {
	if (!hasHour()) {
		throw logic_error("StartTimeDTO::getHour(): !hasHour()");
	}

	return *hour.get();
}

unsigned StartTimeDTO::getMinute() const {
	if (!hasMinute()) {
		throw logic_error("StartTimeDTO::getMinute(): !hasMinute()");
	}

	return *minute.get();
}

StartTimeDTO& StartTimeDTO::setId(unsigned id) {
	this->id.reset(new unsigned(id));
	return *this;
}

StartTimeDTO& StartTimeDTO::setHour(unsigned hour) {
	this->hour.reset(new unsigned(hour));
	return *this;
}

StartTimeDTO& StartTimeDTO::setMinute(unsigned minute) {
	this->minute.reset(new unsigned(minute));
	return *this;
}

ostream& operator<<(ostream& os, const StartTimeDTO& startTime) {
	os << "StartTimeDTO{",
	PRINT_PTR(os, "id", startTime.id.get());
	os << ", ";
	PRINT_PTR(os, "hour", startTime.hour.get());
	os << ", ";
	PRINT_PTR(os, "minute", startTime.minute.get());
	os << "}";

	return os;
}
