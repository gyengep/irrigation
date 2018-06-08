#pragma once
#include <memory>
#include <ostream>



class StartTimeDTO {
	std::unique_ptr<const unsigned> id;
	std::unique_ptr<const unsigned> value;

public:
	StartTimeDTO();
	StartTimeDTO(const StartTimeDTO& other);
	StartTimeDTO(unsigned value);

	bool operator== (const StartTimeDTO& other) const;

	bool hasId() const;
	bool hasValue() const;

	unsigned getId() const;
	unsigned getValue() const;

	StartTimeDTO& setId(unsigned id);
	StartTimeDTO& setValue(unsigned value);


	friend std::ostream& operator<<(std::ostream& os, const StartTimeDTO& startTime);
};
