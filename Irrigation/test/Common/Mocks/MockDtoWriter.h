#pragma once
#include <gmock/gmock.h>
#include <DtoReaderWriter/DtoReaderWriter.h>


class MockDtoWriter : public DtoWriter {
public:
	MOCK_CONST_METHOD0(getContentType, std::string());

	MOCK_METHOD1(save, std::string(const DocumentDto& documentDto));
	MOCK_METHOD1(save, std::string(const ProgramDto& programDto));
	MOCK_METHOD1(save, std::string(const RunTimeDto& runTimeDto));
	MOCK_METHOD1(save, std::string(const StartTimeDto& startTimeDto));
	MOCK_METHOD1(save, std::string(const SchedulersDto& schedulersDto));
	MOCK_METHOD1(save, std::string(const EveryDaySchedulerDto& schedulerDto));
	MOCK_METHOD1(save, std::string(const HotWeatherSchedulerDto& schedulerDto));
	MOCK_METHOD1(save, std::string(const TemperatureDependentSchedulerDto& schedulerDto));
	MOCK_METHOD1(save, std::string(const WeeklySchedulerDto& schedulerDto));
	MOCK_METHOD1(save, std::string(const ProgramDtoList& programDtoList));
	MOCK_METHOD1(save, std::string(const RunTimeDtoList& runTimeDtoList));
	MOCK_METHOD1(save, std::string(const StartTimeDtoList& startTimeDtoList));

	MOCK_METHOD2(save, std::string(const CurrentTemperatureDto& currentTemperatureDto, const std::string& styleSheet));
	MOCK_METHOD2(save, std::string(const TemperatureHistoryDto& temperatureHistoryDto, const std::string& styleSheet));
	MOCK_METHOD2(save, std::string(const TemperatureForecastDto& temperatureForecastDto, const std::string& styleSheet));
	MOCK_METHOD3(save, std::string(const TemperatureHistoryDto& temperatureHistoryDto, const TemperatureForecastDto& temperatureForecastDto, const std::string& styleSheet));

	MOCK_METHOD2(save, std::string(const ProgramDto& programDto, const std::string& styleSheet));
	MOCK_METHOD2(save, std::string(const ProgramDtoList& programDtoList, const std::string& styleSheet));
	MOCK_METHOD2(save, std::string(const LogEntryDtoList& logEntryDtoList, const std::string& styleSheet));
};
