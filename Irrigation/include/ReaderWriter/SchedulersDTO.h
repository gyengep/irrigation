#pragma once
#include <list>
#include <memory>
#include "EqualsPtr.h"



class SpecifiedSchedulerDTO {
	std::unique_ptr<const std::list<bool>> values;

public:
	SpecifiedSchedulerDTO() {
	}

	SpecifiedSchedulerDTO(const SpecifiedSchedulerDTO& other) {
		if (other.hasValues()) {
			setValues(new std::list<bool>(other.getValues()));
		}
	}

	SpecifiedSchedulerDTO(const std::list<bool>* values) {
		setValues(values);
	}

	bool operator== (const SpecifiedSchedulerDTO& other) const {
		return (equalsPtr(values.get(), other.values.get()));
	}

	bool hasValues() const {
		return (values.get() != nullptr);
	}

	const std::list<bool>& getValues() const {
		if (!hasValues()) {
			throw std::logic_error("SpecifiedSchedulerDTO::getValues()");
		}

		return *values.get();
	}

	SpecifiedSchedulerDTO& setValues(const std::list<bool>* values) {
		this->values.reset(values);
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const SpecifiedSchedulerDTO& scheduler) {
	    os << "SpecifiedSchedulerDTO{";
		PRINT_PTR(os, "values", scheduler.values.get());
		os << "}";

	    return os;
	}
};
