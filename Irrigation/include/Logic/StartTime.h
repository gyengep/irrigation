#pragma once
#include <string>
#include "DTO/StartTimeDTO.h"


class StartTime {
	// disable copy constructor and copy operator
	StartTime(const StartTime&);
	StartTime& operator= (const StartTime&);

	unsigned hour, minute, second;

public:
	StartTime();
	StartTime(unsigned hour, unsigned minute, unsigned second = 0);
	virtual ~StartTime();

	bool operator< (const StartTime& other) const;
	bool operator== (const StartTime& other) const;

	void set(unsigned hour, unsigned minute, unsigned second = 0);

	unsigned getHours() const;
	unsigned getMinutes() const;
	unsigned getSeconds() const;

	StartTimeDTO getStartTimeDTO() const;
	void updateFromDTO(const StartTimeDTO& startTimeDTO);

	friend std::string to_string(const StartTime& startTime);
};
