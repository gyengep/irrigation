#pragma once
#include <gmock/gmock.h>
#include <DtoReaderWriter/DtoReaderWriter.h>


class MockDtoWriter : public DtoWriter {
public:
	MOCK_METHOD1(save, std::string(const DocumentDTO& document));
	MOCK_METHOD1(save, std::string(const ProgramDTO& program));
	MOCK_METHOD3(save, std::string(const ProgramDTO& program, const std::string& piName, const std::string& piValue));
	MOCK_METHOD1(save, std::string(const RunTimeDTO& runTime));
	MOCK_METHOD1(save, std::string(const StartTimeDTO& startTime));
	MOCK_METHOD1(save, std::string(const EveryDaySchedulerDTO& scheduler));
	MOCK_METHOD1(save, std::string(const HotWeatherSchedulerDTO& scheduler));
	MOCK_METHOD1(save, std::string(const TemperatureDependentSchedulerDTO& scheduler));
	MOCK_METHOD1(save, std::string(const WeeklySchedulerDTO& scheduler));
	MOCK_METHOD1(save, std::string(const std::list<ProgramDTO>& programs));
	MOCK_METHOD3(save, std::string(const std::list<ProgramDTO>& programs, const std::string& piName, const std::string& piValue));
	MOCK_METHOD1(save, std::string(const std::list<RunTimeDTO>& runTimes));
	MOCK_METHOD1(save, std::string(const std::list<StartTimeDTO>& startTimes));
};
