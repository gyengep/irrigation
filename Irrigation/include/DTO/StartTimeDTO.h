#pragma once
#include <memory>
#include <ostream>



class StartTimeDTO {
	std::unique_ptr<const unsigned> id;
	std::unique_ptr<const unsigned> hour;
	std::unique_ptr<const unsigned> minute;

public:
	StartTimeDTO() = default;
	StartTimeDTO(StartTimeDTO&& other) = default;
	StartTimeDTO(const StartTimeDTO& other);
	StartTimeDTO(unsigned hour, unsigned minute);

	StartTimeDTO& operator= (StartTimeDTO&&) = delete;
	StartTimeDTO& operator= (const StartTimeDTO&) = delete;
	bool operator== (const StartTimeDTO& other) const;

	bool hasId() const;
	bool hasHour() const;
	bool hasMinute() const;

	unsigned getId() const;
	unsigned getHour() const;
	unsigned getMinute() const;

	StartTimeDTO& setId(unsigned id);
	StartTimeDTO& setHour(unsigned hour);
	StartTimeDTO& setMinute(unsigned minute);

	friend std::ostream& operator<<(std::ostream& os, const StartTimeDTO& startTime);
};
