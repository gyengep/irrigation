#pragma once
#include <memory>
#include <ostream>
#include <string>
#include "DTO/StartTimeDTO.h"


class StartTime {
public:
	StartTime() = default;
	StartTime(StartTime&&) = delete;
	StartTime(const StartTime&) = delete;
	virtual ~StartTime() = default;

	StartTime& operator= (StartTime&&) = delete;
	StartTime& operator= (const StartTime&) = delete;
	bool equals(unsigned hour, unsigned minute, unsigned second) const;
	bool operator< (const StartTime& other) const;
	bool operator== (const StartTime& other) const;

	virtual void set(unsigned hour, unsigned minute) = 0;

	virtual unsigned getHours() const = 0;
	virtual unsigned getMinutes() const = 0;

	virtual StartTimeDTO toStartTimeDto() const = 0;
	virtual void updateFromStartTimeDto(const StartTimeDTO& startTimeDTO) = 0;

	friend std::string to_string_short(const StartTime& startTime);
	friend std::string to_string(const StartTime& startTime);
	friend std::ostream& operator<<(std::ostream& os, const StartTime& startTime);
};

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<StartTime> StartTimePtr;
typedef std::shared_ptr<const StartTime> ConstStartTimePtr;

///////////////////////////////////////////////////////////////////////////////

class StartTimeFactory {
public:
	virtual ~StartTimeFactory() = default;
	virtual StartTimePtr create() const;
};
