#pragma once
#include <memory>
#include <string>



class StartTimeDTO {
	std::unique_ptr<const std::string> value;

public:
	StartTimeDTO() : value() {
	}

	StartTimeDTO(const char* value) {
		setValue(value);
	}

	StartTimeDTO(const StartTimeDTO& other) {
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
