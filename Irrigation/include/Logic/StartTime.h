#pragma once
#include <memory>
#include <string>
#include "DTO/StartTimeDTO.h"
#include "Utils/OstreamInsert.h"


class StartTime {
public:
	virtual ~StartTime() = default;

	virtual bool less(const StartTime& other) const = 0;

	virtual void set(unsigned hour, unsigned minute) = 0;

	virtual unsigned getHours() const = 0;
	virtual unsigned getMinutes() const = 0;

	virtual StartTimeDTO toStartTimeDto() const = 0;
	virtual void updateFromStartTimeDto(const StartTimeDTO& startTimeDTO) = 0;

	virtual std::string toString() const = 0;
	virtual std::string toShortString() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(StartTime);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<StartTime> StartTimePtr;
typedef std::shared_ptr<const StartTime> ConstStartTimePtr;

///////////////////////////////////////////////////////////////////////////////

class StartTimeFactory {
public:
	virtual ~StartTimeFactory() = default;
	virtual StartTimePtr create() const = 0;
};
