#pragma once
#include <list>
#include <string>
#include "DTO/DocumentDto.h"
#include "DTO/ProgramDto.h"
#include "DTO/RunTimeDto.h"
#include "DTO/StartTimeDto.h"
#include "DTO/SchedulersDto.h"
#include "DTO/EveryDaySchedulerDto.h"
#include "DTO/HotWeatherSchedulerDto.h"
#include "DTO/TemperatureDependentSchedulerDto.h"
#include "DTO/CurrentTemperatureDto.h"
#include "DTO/TemperatureHistoryDto.h"
#include "DTO/TemperatureForecastDto.h"
#include "DTO/WeeklySchedulerDto.h"
#include "DTO/LogEntryDto.h"


class DtoReader {
public:
	virtual ~DtoReader() = default;
	virtual DocumentDto loadDocument(const std::string& text) const = 0;
	virtual ProgramDto loadProgram(const std::string& text) const = 0;
	virtual RunTimeDto loadRunTime(const std::string& text) const = 0;
	virtual StartTimeDto loadStartTime(const std::string& text) const = 0;
	virtual EveryDaySchedulerDto loadEveryDayScheduler(const std::string& text) const = 0;
	virtual HotWeatherSchedulerDto loadHotWeatherScheduler(const std::string& text) const = 0;
	virtual TemperatureDependentSchedulerDto loadTemperatureDependentScheduler(const std::string& text) const = 0;
	virtual WeeklySchedulerDto loadWeeklyScheduler(const std::string& text) const = 0;
	virtual std::list<ProgramDto> loadProgramList(const std::string& text) const = 0;
	virtual std::list<RunTimeDto> loadRunTimeList(const std::string& text) const = 0;
	virtual std::list<StartTimeDto> loadStartTimeList(const std::string& text) const = 0;
	virtual SchedulersDto loadSchedulers(const std::string& text) const = 0;
};


class DtoWriter {
public:
	virtual ~DtoWriter() = default;

	virtual std::string getContentType() const = 0;

	virtual std::string save(const DocumentDto& document) = 0;
	virtual std::string save(const ProgramDto& program) = 0;
	virtual std::string save(const RunTimeDto& runTime) = 0;
	virtual std::string save(const StartTimeDto& startTime) = 0;
	virtual std::string save(const EveryDaySchedulerDto& scheduler) = 0;
	virtual std::string save(const HotWeatherSchedulerDto& scheduler) = 0;
	virtual std::string save(const TemperatureDependentSchedulerDto& scheduler) = 0;
	virtual std::string save(const WeeklySchedulerDto& scheduler) = 0;
	virtual std::string save(const std::list<ProgramDto>& programs) = 0;
	virtual std::string save(const std::list<RunTimeDto>& runTimes) = 0;
	virtual std::string save(const std::list<StartTimeDto>& startTimes) = 0;
	virtual std::string save(const SchedulersDto& scheduler) = 0;

	virtual std::string save(const CurrentTemperatureDto& currentTemperature, const std::string& styleSheet) = 0;
	virtual std::string save(const TemperatureHistoryDto& temperatureHistory, const std::string& styleSheet) = 0;
	virtual std::string save(const TemperatureForecastDto& temperatureForecast, const std::string& styleSheet) = 0;
	virtual std::string save(const TemperatureHistoryDto& temperatureHistory, const TemperatureForecastDto& temperatureForecast, const std::string& styleSheet) = 0;

	virtual std::string save(const ProgramDto& program, const std::string& styleSheet) = 0;
	virtual std::string save(const std::list<ProgramDto>& programs, const std::string& styleSheet) = 0;
	virtual std::string save(const LogEntryDtoList& logEntries, const std::string& styleSheet) = 0;
};
