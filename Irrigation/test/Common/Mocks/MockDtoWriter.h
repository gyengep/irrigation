#pragma once
#include <gmock/gmock.h>
#include <DtoReaderWriter/DtoReaderWriter.h>


class MockDtoWriter : public DtoWriter {
public:
	MOCK_CONST_METHOD0(getContentType, std::string());

	MOCK_METHOD1(save, std::string(const DocumentDto& document));
	MOCK_METHOD1(save, std::string(const ProgramDto& program));
	MOCK_METHOD1(save, std::string(const RunTimeDto& runTime));
	MOCK_METHOD1(save, std::string(const StartTimeDto& startTime));
	MOCK_METHOD1(save, std::string(const SchedulersDto& scheduler));
	MOCK_METHOD1(save, std::string(const EveryDaySchedulerDto& scheduler));
	MOCK_METHOD1(save, std::string(const HotWeatherSchedulerDto& scheduler));
	MOCK_METHOD1(save, std::string(const TemperatureDependentSchedulerDto& scheduler));
	MOCK_METHOD1(save, std::string(const WeeklySchedulerDto& scheduler));
	MOCK_METHOD1(save, std::string(const std::list<ProgramDto>& programs));
	MOCK_METHOD1(save, std::string(const std::list<RunTimeDto>& runTimes));
	MOCK_METHOD1(save, std::string(const std::list<StartTimeDto>& startTimes));

	MOCK_METHOD2(save, std::string(const CurrentTemperatureDto& currentTemperature, const std::string& styleSheet));
	MOCK_METHOD2(save, std::string(const TemperatureHistoryDto& temperatureHistory, const std::string& styleSheet));
	MOCK_METHOD2(save, std::string(const TemperatureForecastDto& temperatureForecast, const std::string& styleSheet));
	MOCK_METHOD3(save, std::string(const TemperatureHistoryDto& temperatureHistory, const TemperatureForecastDto& temperatureForecast, const std::string& styleSheet));

	MOCK_METHOD2(save, std::string(const ProgramDto& program, const std::string& styleSheet));
	MOCK_METHOD2(save, std::string(const std::list<ProgramDto>& programs, const std::string& styleSheet));
	MOCK_METHOD2(save, std::string(const LogEntryDtoList& logEntries, const std::string& styleSheet));
};
