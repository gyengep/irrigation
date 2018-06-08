#pragma once
#include <memory>
#include "EqualsPtr.h"



class RunTimeDTO {
	std::unique_ptr<const unsigned> id;
	std::unique_ptr<const unsigned> value;

public:
	RunTimeDTO() {
	}

	RunTimeDTO(const RunTimeDTO& other) {
		if (other.hasId()) {
			setId(other.getId());
		}

		if (other.hasValue()) {
			setValue(other.getValue());
		}
	}

	RunTimeDTO(unsigned value) {
		setValue(value);
	}

	bool operator== (const RunTimeDTO& other) const {
		return (equalsPtr(id.get(), other.id.get()) &&
				equalsPtr(value.get(), other.value.get()));
	}

	bool hasId() const {
		return (id.get() != nullptr);
	}

	unsigned getId() const {
		if (!hasId()) {
			throw std::logic_error("RunTimeDTO::getId()");
		}

		return *id.get();
	}

	RunTimeDTO& setId(unsigned id) {
		this->id.reset(new unsigned(id));
		return *this;
	}

	bool hasValue() const {
		return (value.get() != nullptr);
	}

	unsigned getValue() const {
		if (!hasValue()) {
			throw std::logic_error("RunTimeDTO::getValue()");
		}

		return *value.get();
	}

	RunTimeDTO& setValue(unsigned value) {
		this->value.reset(new unsigned(value));
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const RunTimeDTO& runTime) {
	    os << "RunTimeDTO{",
		PRINT_PTR(os, "id", runTime.id.get());
		os << ", ";
		PRINT_PTR(os, "value", runTime.value.get());
	    os << "}";

	    return os;
	}
};
