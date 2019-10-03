#pragma once
#include <list>
#include <memory>
#include <string>
#include "RunTimeDTO.h"
#include "StartTimeDTO.h"
#include "EveryDaySchedulerDTO.h"
#include "HotWeatherSchedulerDTO.h"
#include "PeriodicSchedulerDTO.h"
#include "TemperatureDependentSchedulerDTO.h"
#include "WeeklySchedulerDTO.h"
#include "DTO.h"


class ProgramDTO {

public:
	ProgramDTO() = default;
	ProgramDTO(ProgramDTO&& other) = default;
	ProgramDTO(const ProgramDTO& other);
	ProgramDTO(bool disabled, const std::string& name,
			unsigned adjustment,
			const std::string& schedulerType,
			EveryDaySchedulerDTO&& everyDayScheduler,
			HotWeatherSchedulerDTO&& hotWeatherScheduler,
			PeriodicSchedulerDTO&& periodicScheduler,
			TemperatureDependentSchedulerDTO&& temperatureDependentScheduler,
			WeeklySchedulerDTO&& weeklyScheduler,
			std::list<RunTimeDTO>&& runTimes,
			std::list<StartTimeDTO>&& startTimes);

	ProgramDTO& operator= (ProgramDTO&&) = default;
	ProgramDTO& operator= (const ProgramDTO&) = delete;
	bool operator== (const ProgramDTO& other) const;

	DECLARE_DTO_VALUE_COPY(ProgramDTO, unsigned, Id);
	DECLARE_DTO_VALUE_COPY(ProgramDTO, bool, Disabled);
	DECLARE_DTO_VALUE_COPY(ProgramDTO, std::string, Name);
	DECLARE_DTO_VALUE_COPY(ProgramDTO, unsigned, Adjustment);
	DECLARE_DTO_VALUE_COPY(ProgramDTO, std::string, SchedulerType);
	DECLARE_DTO_VALUE_MOVE(ProgramDTO, EveryDaySchedulerDTO, EveryDayScheduler);
	DECLARE_DTO_VALUE_MOVE(ProgramDTO, HotWeatherSchedulerDTO, HotWeatherScheduler);
	DECLARE_DTO_VALUE_MOVE(ProgramDTO, PeriodicSchedulerDTO, PeriodicScheduler);
	DECLARE_DTO_VALUE_MOVE(ProgramDTO, TemperatureDependentSchedulerDTO, TemperatureDependentScheduler);
	DECLARE_DTO_VALUE_MOVE(ProgramDTO, WeeklySchedulerDTO, WeeklyScheduler);
	DECLARE_DTO_VALUE_MOVE(ProgramDTO, std::list<RunTimeDTO>, RunTimes);
	DECLARE_DTO_VALUE_MOVE(ProgramDTO, std::list<StartTimeDTO>, StartTimes);

	friend std::ostream& operator<<(std::ostream& os, const ProgramDTO& program);
};
