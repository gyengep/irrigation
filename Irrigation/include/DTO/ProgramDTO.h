#pragma once
#include <list>
#include <memory>
#include <string>
#include "RunTimeDTO.h"
#include "StartTimeDTO.h"
#include "PeriodicSchedulerDTO.h"
#include "WeeklySchedulerDTO.h"



class ProgramDTO {
	std::unique_ptr<const unsigned> id;
	std::unique_ptr<const std::string> name;
	std::unique_ptr<const std::string> schedulerType;
	std::unique_ptr<const PeriodicSchedulerDTO> periodicScheduler;
	std::unique_ptr<const WeeklySchedulerDTO> weeklyScheduler;
	std::unique_ptr<const std::list<RunTimeDTO>> runTimes;
	std::unique_ptr<const std::list<StartTimeDTO>> startTimes;

public:
	ProgramDTO() = default;
	ProgramDTO(ProgramDTO&& other) = default;
	ProgramDTO(const ProgramDTO& other);
	ProgramDTO(const std::string& name, const std::string& schedulerType,
			PeriodicSchedulerDTO&& periodicScheduler,
			WeeklySchedulerDTO&& weeklyScheduler,
			std::list<RunTimeDTO>&& runTimes,
			std::list<StartTimeDTO>&& startTimes);

	ProgramDTO& operator= (ProgramDTO&&) = delete;
	ProgramDTO& operator= (const ProgramDTO&) = delete;
	bool operator== (const ProgramDTO& other) const;

	bool hasId() const;
	bool hasName() const;
	bool hasSchedulerType() const;
	bool hasPeriodicScheduler() const;
	bool hasWeeklyScheduler() const;
	bool hasRunTimes() const;
	bool hasStartTimes() const;

	unsigned getId() const;
	const std::string& getName() const;
	const std::string& getSchedulerType() const;
	const PeriodicSchedulerDTO& getPeriodicScheduler() const;
	const WeeklySchedulerDTO& getWeeklyScheduler() const;
	const std::list<RunTimeDTO>& getRunTimes() const;
	const std::list<StartTimeDTO>& getStartTimes() const;

	ProgramDTO& setId(unsigned id);
	ProgramDTO& setName(const std::string& name);
	ProgramDTO& setSchedulerType(const std::string& schedulerType);
	ProgramDTO& setPeriodicScheduler(PeriodicSchedulerDTO&& periodicScheduler);
	ProgramDTO& setWeeklyScheduler(WeeklySchedulerDTO&& weeklyScheduler);
	ProgramDTO& setRunTimes(std::list<RunTimeDTO>&& runTimes);
	ProgramDTO& setStartTimes(std::list<StartTimeDTO>&& startTimes);

	friend std::ostream& operator<<(std::ostream& os, const ProgramDTO& program);
};
