#pragma once
#include <gmock/gmock.h>
#include <DtoReaderWriter/DtoReaderWriter.h>


class MockDtoReader : public DtoReader {
public:
	MOCK_CONST_METHOD1(loadDocument, DocumentDTO(const std::string&));
	MOCK_CONST_METHOD1(loadProgram, ProgramDTO(const std::string&));
	MOCK_CONST_METHOD1(loadRunTime, RunTimeDTO(const std::string&));
	MOCK_CONST_METHOD1(loadStartTime, StartTimeDTO(const std::string&));
	MOCK_CONST_METHOD1(loadEveryDayScheduler, EveryDaySchedulerDTO(const std::string&));
	MOCK_CONST_METHOD1(loadHotWeatherScheduler, HotWeatherSchedulerDTO(const std::string&));
	MOCK_CONST_METHOD1(loadTemperatureDependentScheduler, TemperatureDependentSchedulerDTO(const std::string&));
	MOCK_CONST_METHOD1(loadWeeklyScheduler, WeeklySchedulerDTO(const std::string&));
	MOCK_CONST_METHOD1(loadProgramList, std::list<ProgramDTO>(const std::string& text));
	MOCK_CONST_METHOD1(loadRunTimeList, std::list<RunTimeDTO>(const std::string& text));
	MOCK_CONST_METHOD1(loadStartTimeList, std::list<StartTimeDTO>(const std::string& text));
};
