#pragma once
#include <list>
#include <memory>
#include <string>



class SpecifiedSchedulerDTO {
	std::unique_ptr<const std::list<std::string>> values;

public:
	SpecifiedSchedulerDTO() : values() {
	}

	SpecifiedSchedulerDTO(const std::list<std::string>* values) {
		setValues(values);
	}

	const std::list<std::string>* getValues() const {
		return values.get();
	}

	void setValues(const std::list<std::string>* values) {
		this->values.reset(values);
	}
};
