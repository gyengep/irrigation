#pragma once
#include <memory>
#include <ostream>



class StartTimeDTO {
	std::unique_ptr<const unsigned> id;
	std::unique_ptr<const unsigned> hours;
	std::unique_ptr<const unsigned> minutes;

public:
	StartTimeDTO() = default;
	StartTimeDTO(StartTimeDTO&& other) = default;
	StartTimeDTO(const StartTimeDTO& other);
	StartTimeDTO(unsigned hour, unsigned minute);

	StartTimeDTO& operator= (StartTimeDTO&&) = delete;
	StartTimeDTO& operator= (const StartTimeDTO&) = delete;
	bool operator== (const StartTimeDTO& other) const;

	bool hasId() const;
	bool hasHours() const;
	bool hasMinutes() const;

	unsigned getId() const;
	unsigned getHours() const;
	unsigned getMinutes() const;

	StartTimeDTO& setId(unsigned id);
	StartTimeDTO& setHours(unsigned hour);
	StartTimeDTO& setMinutes(unsigned minute);

	friend std::ostream& operator<<(std::ostream& os, const StartTimeDTO& startTime);
};
