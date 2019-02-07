#include "StartTimeDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


StartTimeDTO::StartTimeDTO(const StartTimeDTO& other) {
	if (other.hasId()) {
		setId(other.getId());
	}

	if (other.hasHours()) {
		setHours(other.getHours());
	}

	if (other.hasMinutes()) {
		setMinutes(other.getMinutes());
	}
}

StartTimeDTO::StartTimeDTO(unsigned hours, unsigned minutes) {
	setHours(hours);
	setMinutes(minutes);
}

bool StartTimeDTO::operator== (const StartTimeDTO& other) const {
	return (equalsPtr(id.get(), other.id.get()) &&
			equalsPtr(hours.get(), other.hours.get()) &&
			equalsPtr(minutes.get(), other.minutes.get()));
}

bool StartTimeDTO::hasId() const {
	return (id.get() != nullptr);
}

bool StartTimeDTO::hasHours() const {
	return (hours.get() != nullptr);
}

bool StartTimeDTO::hasMinutes() const {
	return (minutes.get() != nullptr);
}

unsigned StartTimeDTO::getId() const {
	if (!hasId()) {
		throw logic_error("StartTimeDTO::getId(): !hasId()");
	}

	return *id.get();
}

unsigned StartTimeDTO::getHours() const {
	if (!hasHours()) {
		throw logic_error("StartTimeDTO::getHours(): !hasHours()");
	}

	return *hours.get();
}

unsigned StartTimeDTO::getMinutes() const {
	if (!hasMinutes()) {
		throw logic_error("StartTimeDTO::getMinutes(): !hasMinutes()");
	}

	return *minutes.get();
}

StartTimeDTO& StartTimeDTO::setId(unsigned id) {
	this->id.reset(new unsigned(id));
	return *this;
}

StartTimeDTO& StartTimeDTO::setHours(unsigned hours) {
	this->hours.reset(new unsigned(hours));
	return *this;
}

StartTimeDTO& StartTimeDTO::setMinutes(unsigned minutes) {
	this->minutes.reset(new unsigned(minutes));
	return *this;
}

ostream& operator<<(ostream& os, const StartTimeDTO& startTime) {
	os << "StartTimeDTO{",
	PRINT_PTR(os, "id", startTime.id.get());
	os << ", ";
	PRINT_PTR(os, "hours", startTime.hours.get());
	os << ", ";
	PRINT_PTR(os, "minutes", startTime.minutes.get());
	os << "}";

	return os;
}
