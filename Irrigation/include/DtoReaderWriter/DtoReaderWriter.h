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
	virtual DocumentDto loadDocumentDto(const std::string& text) const = 0;
	virtual ProgramDto loadProgramDto(const std::string& text) const = 0;
	virtual RunTimeDto loadRunTimeDto(const std::string& text) const = 0;
	virtual StartTimeDto loadStartTimeDto(const std::string& text) const = 0;
	virtual EveryDaySchedulerDto loadEveryDaySchedulerDto(const std::string& text) const = 0;
	virtual HotWeatherSchedulerDto loadHotWeatherSchedulerDto(const std::string& text) const = 0;
	virtual TemperatureDependentSchedulerDto loadTemperatureDependentSchedulerDto(const std::string& text) const = 0;
	virtual WeeklySchedulerDto loadWeeklySchedulerDto(const std::string& text) const = 0;
	virtual ProgramDtoList loadProgramDtoList(const std::string& text) const = 0;
	virtual RunTimeDtoList loadRunTimeDtoList(const std::string& text) const = 0;
	virtual StartTimeDtoList loadStartTimeDtoList(const std::string& text) const = 0;
	virtual SchedulersDto loadSchedulersDto(const std::string& text) const = 0;
};


class DtoWriter {
public:
	virtual ~DtoWriter() = default;

	virtual std::string getContentType() const = 0;

	virtual std::string save(const DocumentDto& documentDto) = 0;
	virtual std::string save(const ProgramDto& programDto) = 0;
	virtual std::string save(const RunTimeDto& runTimeDto) = 0;
	virtual std::string save(const StartTimeDto& startTimeDto) = 0;
	virtual std::string save(const EveryDaySchedulerDto& schedulerDto) = 0;
	virtual std::string save(const HotWeatherSchedulerDto& schedulerDto) = 0;
	virtual std::string save(const TemperatureDependentSchedulerDto& schedulerDto) = 0;
	virtual std::string save(const WeeklySchedulerDto& schedulerDto) = 0;
	virtual std::string save(const ProgramDtoList& programDtoList) = 0;
	virtual std::string save(const RunTimeDtoList& runTimeDtoList) = 0;
	virtual std::string save(const StartTimeDtoList& startTimeDtoList) = 0;
	virtual std::string save(const SchedulersDto& schedulersDto) = 0;

	virtual std::string save(const CurrentTemperatureDto& currentTemperatureDto, const std::string& styleSheet) = 0;
	virtual std::string save(const TemperatureHistoryDto& temperatureHistoryDto, const std::string& styleSheet) = 0;
	virtual std::string save(const TemperatureForecastDto& temperatureForecastDto, const std::string& styleSheet) = 0;
	virtual std::string save(const TemperatureHistoryDto& temperatureHistoryDto, const TemperatureForecastDto& temperatureForecast, const std::string& styleSheet) = 0;

	virtual std::string save(const ProgramDto& programDto, const std::string& styleSheet) = 0;
	virtual std::string save(const ProgramDtoList& programDtoList, const std::string& styleSheet) = 0;
	virtual std::string save(const LogEntryDtoList& logEntryDtoList, const std::string& styleSheet) = 0;
};
