#pragma once
#include <memory>
#include <string>



class RunTimeDTO {
	std::unique_ptr<const std::string> value;

public:
	RunTimeDTO() : value() {
	}

	RunTimeDTO(const char* value) {
		setValue(value);
	}

	RunTimeDTO(const RunTimeDTO& other) {
		if (other.getValue() != nullptr) {
			setValue(other.getValue());
		}
	}

	const char* getValue() const {
		return value.get() ? value.get()->c_str() : NULL;
	}

	void setValue(const char* value) {
		this->value.reset(new std::string(value));
	}
};
