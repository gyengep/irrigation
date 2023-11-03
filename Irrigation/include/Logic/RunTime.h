#pragma once
#include <chrono>
#include <memory>
#include <string>
#include "DTO/RunTimeDto.h"
#include "Utils/OstreamInsert.h"


class RunTime {
public:
	virtual ~RunTime() = default;

	virtual std::chrono::seconds get() const = 0;
	virtual void set(const std::chrono::seconds& time) = 0;	// throws ValueOutOfBoundsException

	virtual RunTimeDto toRunTimeDto() const = 0;
	virtual void updateFromRunTimeDto(const RunTimeDto& runTimeDto) = 0; // throws ValueOutOfBoundsException

	virtual std::string toString() const = 0;
	virtual std::chrono::seconds toDuration() const = 0;

	static std::chrono::seconds toDuration(const RunTimeDto& runTimeDto);
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(RunTime);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<RunTime> RunTimePtr;
typedef std::shared_ptr<const RunTime> ConstRunTimePtr;

///////////////////////////////////////////////////////////////////////////////

class RunTimeFactory {
public:
	virtual ~RunTimeFactory() = default;
	virtual RunTimePtr create() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<RunTimeFactory> RunTimeFactoryPtr;
