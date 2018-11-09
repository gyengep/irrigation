#pragma once
#include <string>
#include "DTO/StartTimeDTO.h"


class StartTime {

	unsigned hour, minute, second;

public:
	StartTime();
	StartTime(StartTime&&) = default;
	StartTime(const StartTime&) = default;
	StartTime(unsigned hour, unsigned minute, unsigned second = 0);
	virtual ~StartTime();

	bool operator< (const StartTime& other) const;
	bool operator== (const StartTime& other) const;
	StartTime& operator= (StartTime&&) = default;
	StartTime& operator= (const StartTime&) = default;

	void set(unsigned hour, unsigned minute, unsigned second = 0);

	unsigned getHours() const;
	unsigned getMinutes() const;
	unsigned getSeconds() const;

	StartTimeDTO getStartTimeDTO() const;
	void updateFromDTO(const StartTimeDTO& startTimeDTO);

	friend std::string to_string(const StartTime& startTime);
};
