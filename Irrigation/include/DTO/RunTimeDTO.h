#pragma once
#include <memory>
#include <ostream>



class RunTimeDTO {
	std::unique_ptr<const unsigned> id;
	std::unique_ptr<const unsigned> value;

public:
	RunTimeDTO();
	RunTimeDTO(const RunTimeDTO& other);
	RunTimeDTO(unsigned value);

	bool operator== (const RunTimeDTO& other) const;

	bool hasId() const;
	bool hasValue() const;

	unsigned getId() const;
	unsigned getValue() const;

	RunTimeDTO& setId(unsigned id);
	RunTimeDTO& setValue(unsigned value);

	friend std::ostream& operator<<(std::ostream& os, const RunTimeDTO& runTime);
};
