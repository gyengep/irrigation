#pragma once
#include <string>
#include "DTO/StartTimeDTO.h"


class StartTime {

	unsigned hour, minute;
	static const unsigned second = 0;

public:
	StartTime();
	StartTime(StartTime&&) = default;
	StartTime(const StartTime&) = default;
	StartTime(unsigned hour, unsigned minute);
	virtual ~StartTime();

	StartTime& operator= (StartTime&&) = delete;
	StartTime& operator= (const StartTime&) = delete;
	bool equals(unsigned hour, unsigned minute, unsigned second) const;
	bool operator< (const StartTime& other) const;
	bool operator== (const StartTime& other) const;

	void set(unsigned hour, unsigned minute);

	unsigned getHours() const;
	unsigned getMinutes() const;

	StartTimeDTO toStartTimeDto() const;
	void updateFromStartTimeDto(const StartTimeDTO& startTimeDTO);

	friend std::string to_string(const StartTime& startTime);
	friend std::ostream& operator<<(std::ostream& os, const StartTime& startTime);
};
