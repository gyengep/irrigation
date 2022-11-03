#pragma once
#include <ctime>
#include <memory>
#include <string>
#include "json.hpp"
#include "DTO/ProgramDTO.h"
#include "Utils/OstreamInsert.h"
#include "RunTimeContainer.h"
#include "StartTimeContainer.h"
#include "SchedulerContainer.h"


class ScheduledResult {
	const bool scheduled;
	const unsigned adjustment;

public:
	ScheduledResult(bool scheduled, unsigned adjustment);

	bool isScheduled() const;
	unsigned getAdjustment() const;

	bool operator==(const ScheduledResult& other) const;
};


class Program {
public:
	Program() = default;
	Program(Program&&) = delete;
	Program(const Program&) = delete;

	virtual ~Program() = default;

	Program& operator= (Program&&) = delete;
	Program& operator= (const Program&) = delete;
	bool operator== (const Program&) const;

	virtual void setEnabled(bool enabled) = 0;
	virtual void setName(const std::string& name) = 0;
	virtual void setAdjustment(unsigned adjustment) = 0;
	virtual void setSchedulerType(SchedulerType schedulerType) = 0;

	virtual bool isEnabled() const = 0;
	virtual const std::string& getName() const = 0;
	virtual unsigned getAdjustment() const = 0;
	virtual SchedulerType getSchedulerType() const = 0;

	virtual std::unique_ptr<ScheduledResult> isScheduled(const std::time_t rawtime) = 0;
//	virtual const Scheduler& getCurrentScheduler() const { return *currentScheduler; }
	virtual Scheduler& getCurrentScheduler() = 0;

	virtual const SchedulerContainer& getSchedulerContainer() const = 0;
	virtual const RunTimeContainer& getRunTimeContainer() const = 0;
	virtual const StartTimeContainer& getStartTimeContainer() const = 0;

	virtual SchedulerContainer& getSchedulerContainer() = 0;
	virtual RunTimeContainer& getRunTimeContainer() = 0;
	virtual StartTimeContainer& getStartTimeContainer() = 0;

	virtual std::pair<IdType, StartTimePtr> createStartTime(const StartTimeDTO& startTimeDto) = 0;

	virtual ProgramDTO toProgramDto() const = 0;
	virtual void updateFromProgramDto(const ProgramDTO& programDTO) = 0;

	virtual std::string toString() const = 0;

	virtual nlohmann::json saveTo() const = 0;
	virtual void loadFrom(const nlohmann::json& values) = 0;
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(Program);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<Program> ProgramPtr;
typedef std::shared_ptr<const Program> ConstProgramPtr;

///////////////////////////////////////////////////////////////////////////////

class ProgramFactory {
public:
	virtual ~ProgramFactory() = default;
	virtual ProgramPtr create() const = 0;
};
