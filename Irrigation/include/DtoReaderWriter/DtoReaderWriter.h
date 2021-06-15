#pragma once
#include <list>
#include <string>
#include "DTO/DocumentDTO.h"
#include "DTO/ProgramDTO.h"
#include "DTO/RunTimeDTO.h"
#include "DTO/StartTimeDTO.h"
#include "DTO/EveryDaySchedulerDTO.h"
#include "DTO/HotWeatherSchedulerDTO.h"
#include "DTO/PeriodicSchedulerDTO.h"
#include "DTO/TemperatureDependentSchedulerDTO.h"
#include "DTO/WeeklySchedulerDTO.h"


class DtoReader {
public:
	virtual ~DtoReader() = default;
	virtual DocumentDTO loadDocument(const std::string& text) const = 0;
	virtual ProgramDTO loadProgram(const std::string& text) const = 0;
	virtual RunTimeDTO loadRunTime(const std::string& text) const = 0;
	virtual StartTimeDTO loadStartTime(const std::string& text) const = 0;
	virtual EveryDaySchedulerDTO loadEveryDayScheduler(const std::string& text) const = 0;
	virtual HotWeatherSchedulerDTO loadHotWeatherScheduler(const std::string& text) const = 0;
	virtual PeriodicSchedulerDTO loadPeriodicScheduler(const std::string& text) const = 0;
	virtual TemperatureDependentSchedulerDTO loadTemperatureDependentScheduler(const std::string& text) const = 0;
	virtual WeeklySchedulerDTO loadWeeklyScheduler(const std::string& text) const = 0;
	virtual std::list<ProgramDTO> loadProgramList(const std::string& text) const = 0;
	virtual std::list<RunTimeDTO> loadRunTimeList(const std::string& text) const = 0;
	virtual std::list<StartTimeDTO> loadStartTimeList(const std::string& text) const = 0;
};


class DtoWriter {
public:
	virtual ~DtoWriter() = default;
	virtual std::string save(const DocumentDTO& document) = 0;
	virtual std::string save(const ProgramDTO& program) = 0;
	virtual std::string save(const ProgramDTO& program, const std::string& piName, const std::string& piValue) = 0;
	virtual std::string save(const RunTimeDTO& runTime) = 0;
	virtual std::string save(const StartTimeDTO& startTime) = 0;
	virtual std::string save(const EveryDaySchedulerDTO& scheduler) = 0;
	virtual std::string save(const HotWeatherSchedulerDTO& scheduler) = 0;
	virtual std::string save(const PeriodicSchedulerDTO& scheduler) = 0;
	virtual std::string save(const TemperatureDependentSchedulerDTO& scheduler) = 0;
	virtual std::string save(const WeeklySchedulerDTO& scheduler) = 0;
	virtual std::string save(const std::list<ProgramDTO>& programs) = 0;
	virtual std::string save(const std::list<ProgramDTO>& programs, const std::string& piName, const std::string& piValue) = 0;
	virtual std::string save(const std::list<RunTimeDTO>& runTimes) = 0;
	virtual std::string save(const std::list<StartTimeDTO>& startTimes) = 0;
};
