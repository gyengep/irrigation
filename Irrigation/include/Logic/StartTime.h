#pragma once
#include <string>
#include "DTO/StartTimeDTO.h"


class StartTime {

	unsigned hour, minute, second;

public:
	StartTime();
	StartTime(const StartTime&) = default;
	StartTime(unsigned hour, unsigned minute, unsigned second = 0);
	virtual ~StartTime();

	void set(unsigned hour, unsigned minute, unsigned second = 0);

	unsigned getHours() const;
	unsigned getMinutes() const;
	unsigned getSeconds() const;

	StartTimeDTO getStartTimeDTO() const;
	void updateFromDTO(const StartTimeDTO& startTimeDTO);

	bool operator< (const StartTime& other) const;
	bool operator== (const StartTime& other) const;
	StartTime& operator= (const StartTime&) = default;

	friend std::string to_string(const StartTime& startTime);
};
