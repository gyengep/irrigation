#pragma once
#include <memory>
#include "EqualsPtr.h"



class StartTimeDTO {
	std::unique_ptr<const unsigned> id;
	std::unique_ptr<const unsigned> value;

public:
	StartTimeDTO() {
	}

	StartTimeDTO(const StartTimeDTO& other) {
		if (other.hasId()) {
			setId(other.getId());
		}

		if (other.hasValue()) {
			setValue(other.getValue());
		}
	}

	StartTimeDTO(unsigned value) {
		setValue(value);
	}

	bool operator== (const StartTimeDTO& other) const {
		return (equalsPtr(id.get(), other.id.get()) &&
				equalsPtr(value.get(), other.value.get()));
	}

	bool hasId() const {
		return (id.get() != nullptr);
	}

	unsigned getId() const {
		if (!hasId()) {
			throw std::logic_error("StartTimeDTO::getId()");
		}

		return *id.get();
	}

	StartTimeDTO& setId(unsigned id) {
		this->id.reset(new unsigned(id));
		return *this;
	}

	bool hasValue() const {
		return (value.get() != nullptr);
	}

	unsigned getValue() const {
		if (!hasValue()) {
			throw std::logic_error("StartTimeDTO::getValue()");
		}

		return *value.get();
	}

	StartTimeDTO& setValue(unsigned value) {
		this->value.reset(new unsigned(value));
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const StartTimeDTO& startTime) {
	    os << "StartTimeDTO{",
		PRINT_PTR(os, "id", startTime.id.get());
		os << ", ";
		PRINT_PTR(os, "value", startTime.value.get());
	    os << "}";

	    return os;
	}
};
