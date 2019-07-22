#pragma once
#include <list>
#include <memory>
#include <string>
#include "RunTimeDTO.h"
#include "StartTimeDTO.h"
#include "PeriodicSchedulerDTO.h"
#include "WeeklySchedulerDTO.h"
#include "EveryDaySchedulerDTO.h"


class ProgramDTO {
	std::unique_ptr<const unsigned> id;
	std::unique_ptr<const bool> disabled;
	std::unique_ptr<const std::string> name;
	std::unique_ptr<const unsigned> adjustment;
	std::unique_ptr<const std::string> schedulerType;
	std::unique_ptr<const PeriodicSchedulerDTO> periodicScheduler;
	std::unique_ptr<const WeeklySchedulerDTO> weeklyScheduler;
	std::unique_ptr<const EveryDaySchedulerDTO> everyDayScheduler;
	std::unique_ptr<const std::list<RunTimeDTO>> runTimes;
	std::unique_ptr<const std::list<StartTimeDTO>> startTimes;

public:
	ProgramDTO() = default;
	ProgramDTO(ProgramDTO&& other) = default;
	ProgramDTO(const ProgramDTO& other);
	ProgramDTO(bool disabled, const std::string& name,
			unsigned adjustment,
			const std::string& schedulerType,
			PeriodicSchedulerDTO&& periodicScheduler,
			WeeklySchedulerDTO&& weeklyScheduler,
			EveryDaySchedulerDTO&& everyDayScheduler,
			std::list<RunTimeDTO>&& runTimes,
			std::list<StartTimeDTO>&& startTimes);

	ProgramDTO& operator= (ProgramDTO&&) = default;
	ProgramDTO& operator= (const ProgramDTO&) = delete;
	bool operator== (const ProgramDTO& other) const;

	bool hasId() const;
	bool hasDisabled() const;
	bool hasName() const;
	bool hasAdjustment() const;
	bool hasSchedulerType() const;
	bool hasPeriodicScheduler() const;
	bool hasWeeklyScheduler() const;
	bool hasEveryDayScheduler() const;
	bool hasRunTimes() const;
	bool hasStartTimes() const;

	unsigned getId() const;
	bool getDisabled() const;
	const std::string& getName() const;
	unsigned getAdjustment() const;
	const std::string& getSchedulerType() const;
	const PeriodicSchedulerDTO& getPeriodicScheduler() const;
	const WeeklySchedulerDTO& getWeeklyScheduler() const;
	const EveryDaySchedulerDTO& getEveryDayScheduler() const;
	const std::list<RunTimeDTO>& getRunTimes() const;
	const std::list<StartTimeDTO>& getStartTimes() const;

	ProgramDTO& setId(unsigned id);
	ProgramDTO& setDisabled(bool disabled);
	ProgramDTO& setName(const std::string& name);
	ProgramDTO& setAdjustment(unsigned adjustment);
	ProgramDTO& setSchedulerType(const std::string& schedulerType);
	ProgramDTO& setPeriodicScheduler(PeriodicSchedulerDTO&& periodicScheduler);
	ProgramDTO& setWeeklyScheduler(WeeklySchedulerDTO&& weeklyScheduler);
	ProgramDTO& setEveryDayScheduler(EveryDaySchedulerDTO&& everyDayScheduler);
	ProgramDTO& setRunTimes(std::list<RunTimeDTO>&& runTimes);
	ProgramDTO& setStartTimes(std::list<StartTimeDTO>&& startTimes);

	friend std::ostream& operator<<(std::ostream& os, const ProgramDTO& program);
};
