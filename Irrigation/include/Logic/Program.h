#pragma once
#include <memory>
#include <string>
#include "DTO/ProgramDTO.h"
#include "Utils/DateTime.h"
#include "Utils/OstreamInsert.h"
#include "RunTimeContainer.h"
#include "StartTimeContainer.h"
#include "SchedulerContainer.h"


class Program {
public:
	virtual ~Program() = default;

	virtual void setEnabled(bool enabled) = 0;
	virtual void setName(const std::string& name) = 0;
	virtual void setAdjustment(unsigned adjustment) = 0;
	virtual void setSchedulerType(SchedulerType schedulerType) = 0;

	virtual bool isEnabled() const = 0;
	virtual const std::string& getName() const = 0;
	virtual unsigned getAdjustment() const = 0;
	virtual SchedulerType getSchedulerType() const = 0;

	virtual std::unique_ptr<Scheduler::Result> isScheduled(const LocalDateTime& localDateTime) = 0;


	virtual const SchedulerContainer& getSchedulerContainer() const = 0;
	virtual const RunTimeContainer& getRunTimeContainer() const = 0;
	virtual const StartTimeContainer& getStartTimeContainer() const = 0;

	virtual SchedulerContainer& getSchedulerContainer() = 0;
	virtual RunTimeContainer& getRunTimeContainer() = 0;
	virtual StartTimeContainer& getStartTimeContainer() = 0;

	virtual ProgramDTO toProgramDto() const = 0;
	virtual void updateFromProgramDto(const ProgramDTO& programDTO) = 0;

	virtual std::string toString() const = 0;
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
	virtual ProgramPtr create(unsigned id) const = 0;
};

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<ProgramFactory> ProgramFactoryPtr;
