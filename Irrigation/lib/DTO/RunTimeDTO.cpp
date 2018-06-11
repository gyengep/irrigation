#include "RunTimeDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


RunTimeDTO::RunTimeDTO() {
}

RunTimeDTO::RunTimeDTO(const RunTimeDTO& other) {
	if (other.hasId()) {
		setId(other.getId());
	}

	if (other.hasValue()) {
		setValue(other.getValue());
	}
}

RunTimeDTO::RunTimeDTO(unsigned value) {
	setValue(value);
}

bool RunTimeDTO::operator== (const RunTimeDTO& other) const {
	return (equalsPtr(id.get(), other.id.get()) &&
			equalsPtr(value.get(), other.value.get()));
}

bool RunTimeDTO::hasId() const {
	return (id.get() != nullptr);
}

bool RunTimeDTO::hasValue() const {
	return (value.get() != nullptr);
}

unsigned RunTimeDTO::getId() const {
	if (!hasId()) {
		throw logic_error("RunTimeDTO::getId()");
	}

	return *id.get();
}

unsigned RunTimeDTO::getValue() const {
	if (!hasValue()) {
		throw logic_error("RunTimeDTO::getValue()");
	}

	return *value.get();
}

RunTimeDTO& RunTimeDTO::setId(unsigned id) {
	this->id.reset(new unsigned(id));
	return *this;
}

RunTimeDTO& RunTimeDTO::setValue(unsigned value) {
	this->value.reset(new unsigned(value));
	return *this;
}

ostream& operator<<(ostream& os, const RunTimeDTO& runTime) {
	os << "RunTimeDTO{",
	PRINT_PTR(os, "id", runTime.id.get());
	os << ", ";
	PRINT_PTR(os, "value", runTime.value.get());
	os << "}";

	return os;
}
