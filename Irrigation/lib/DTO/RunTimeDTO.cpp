#include "RunTimeDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


RunTimeDTO::RunTimeDTO(const RunTimeDTO& other) {
	if (other.hasId()) {
		setId(other.getId());
	}

	if (other.hasMinutes()) {
		setMinutes(other.getMinutes());
	}

	if (other.hasSeconds()) {
		setSeconds(other.getSeconds());
	}
}

RunTimeDTO::RunTimeDTO(unsigned minutes, unsigned seconds) {
	setMinutes(minutes);
	setSeconds(seconds);
}

bool RunTimeDTO::operator== (const RunTimeDTO& other) const {
	return (equalsPtr(id.get(), other.id.get()) &&
			equalsPtr(minutes.get(), other.minutes.get()) &&
			equalsPtr(seconds.get(), other.seconds.get()));
}

bool RunTimeDTO::hasId() const {
	return (id.get() != nullptr);
}

bool RunTimeDTO::hasMinutes() const {
	return (minutes.get() != nullptr);
}

bool RunTimeDTO::hasSeconds() const {
	return (seconds.get() != nullptr);
}

unsigned RunTimeDTO::getId() const {
	if (!hasId()) {
		throw logic_error("RunTimeDTO::getId(): !hasId()");
	}

	return *id.get();
}

unsigned RunTimeDTO::getMinutes() const {
	if (!hasMinutes()) {
		throw logic_error("RunTimeDTO::getMinutes(): !hasMinutes()");
	}

	return *minutes.get();
}

unsigned RunTimeDTO::getSeconds() const {
	if (!hasSeconds()) {
		throw logic_error("RunTimeDTO::getSeconds(): !hasSeconds()");
	}

	return *seconds.get();
}

RunTimeDTO& RunTimeDTO::setId(unsigned id) {
	this->id.reset(new unsigned(id));
	return *this;
}

RunTimeDTO& RunTimeDTO::setMinutes(unsigned minutes) {
	this->minutes.reset(new unsigned(minutes));
	return *this;
}

RunTimeDTO& RunTimeDTO::setSeconds(unsigned seconds) {
	this->seconds.reset(new unsigned(seconds));
	return *this;
}

ostream& operator<<(ostream& os, const RunTimeDTO& runTime) {
	os << "RunTimeDTO{",
	PRINT_PTR(os, "id", runTime.id.get());
	os << ", ";
	PRINT_PTR(os, "minutes", runTime.minutes.get());
	os << ", ";
	PRINT_PTR(os, "seconds", runTime.seconds.get());
	os << "}";

	return os;
}
