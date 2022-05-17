#pragma once
#include <memory>
#include <string>
#include "DTO/RunTimeDTO.h"
#include "Utils/OstreamInsert.h"


class RunTime {
public:
	RunTime() = default;
	RunTime(RunTime&&) = delete;
	RunTime(const RunTime&) = delete;
	virtual ~RunTime() = default;

	RunTime& operator= (RunTime&&) = delete;
	RunTime& operator= (const RunTime&) = delete;
	bool operator== (const RunTime& other) const;

	virtual unsigned getSeconds() const = 0;
	virtual unsigned getMilliSeconds() const = 0;
	virtual void setSeconds(unsigned seconds) = 0;
	virtual void setMilliSeconds(unsigned milliSeconds) = 0;

	virtual RunTimeDTO toRunTimeDto() const = 0;
	virtual void updateFromRunTimeDto(const RunTimeDTO& runTimeDTO) = 0;

	virtual std::string toString() const = 0;
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
	virtual RunTimePtr create() const;
};
