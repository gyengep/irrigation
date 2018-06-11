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

	if (other.hasValue()) {
		setValue(other.getValue());
	}
}

StartTimeDTO::StartTimeDTO(unsigned value) {
	setValue(value);
}

bool StartTimeDTO::operator== (const StartTimeDTO& other) const {
	return (equalsPtr(id.get(), other.id.get()) &&
			equalsPtr(value.get(), other.value.get()));
}

bool StartTimeDTO::hasId() const {
	return (id.get() != nullptr);
}

bool StartTimeDTO::hasValue() const {
	return (value.get() != nullptr);
}

unsigned StartTimeDTO::getId() const {
	if (!hasId()) {
		throw logic_error("StartTimeDTO::getId()");
	}

	return *id.get();
}

unsigned StartTimeDTO::getValue() const {
	if (!hasValue()) {
		throw logic_error("StartTimeDTO::getValue()");
	}

	return *value.get();
}

StartTimeDTO& StartTimeDTO::setId(unsigned id) {
	this->id.reset(new unsigned(id));
	return *this;
}

StartTimeDTO& StartTimeDTO::setValue(unsigned value) {
	this->value.reset(new unsigned(value));
	return *this;
}

ostream& operator<<(ostream& os, const StartTimeDTO& startTime) {
	os << "StartTimeDTO{",
	PRINT_PTR(os, "id", startTime.id.get());
	os << ", ";
	PRINT_PTR(os, "value", startTime.value.get());
	os << "}";

	return os;
}
