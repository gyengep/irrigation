#pragma once
#include <memory>
#include <ostream>



class RunTimeDTO {
	std::unique_ptr<const unsigned> id;
	std::unique_ptr<const unsigned> minutes;
	std::unique_ptr<const unsigned> seconds;

public:
	RunTimeDTO();
	RunTimeDTO(const RunTimeDTO& other);
	RunTimeDTO(unsigned minutes, unsigned seconds);

	bool operator== (const RunTimeDTO& other) const;

	bool hasId() const;
	bool hasMinutes() const;
	bool hasSeconds() const;

	unsigned getId() const;
	unsigned getMinutes() const;
	unsigned getSeconds() const;

	RunTimeDTO& setId(unsigned id);
	RunTimeDTO& setMinutes(unsigned minutes);
	RunTimeDTO& setSeconds(unsigned seconds);

	friend std::ostream& operator<<(std::ostream& os, const RunTimeDTO& runTime);
};
