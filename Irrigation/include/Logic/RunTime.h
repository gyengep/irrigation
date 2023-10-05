#pragma once
#include <chrono>
#include <memory>
#include <string>
#include "DTO/RunTimeDTO.h"
#include "Utils/OstreamInsert.h"


class RunTime {
public:
	virtual ~RunTime() = default;

	virtual unsigned getSeconds() const = 0;
	virtual void setSeconds(unsigned seconds) = 0;

	virtual RunTimeDTO toRunTimeDto() const = 0;
	virtual void updateFromRunTimeDto(const RunTimeDTO& runTimeDTO) = 0;

	virtual std::string toString() const = 0;
	virtual std::chrono::seconds toDuration() const = 0;

	static std::chrono::seconds toDuration(const RunTimeDTO& runTimeDTO);
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
