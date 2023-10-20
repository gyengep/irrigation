#pragma once
#include <gmock/gmock.h>
#include <DtoReaderWriter/DtoReaderWriter.h>


class MockDtoReader : public DtoReader {
public:
	MOCK_CONST_METHOD1(loadDocument, DocumentDto(const std::string&));
	MOCK_CONST_METHOD1(loadProgram, ProgramDto(const std::string&));
	MOCK_CONST_METHOD1(loadRunTime, RunTimeDto(const std::string&));
	MOCK_CONST_METHOD1(loadStartTime, StartTimeDto(const std::string&));
	MOCK_CONST_METHOD1(loadSchedulers, SchedulersDto(const std::string& text));
	MOCK_CONST_METHOD1(loadEveryDayScheduler, EveryDaySchedulerDto(const std::string&));
	MOCK_CONST_METHOD1(loadHotWeatherScheduler, HotWeatherSchedulerDto(const std::string&));
	MOCK_CONST_METHOD1(loadTemperatureDependentScheduler, TemperatureDependentSchedulerDto(const std::string&));
	MOCK_CONST_METHOD1(loadWeeklyScheduler, WeeklySchedulerDto(const std::string&));
	MOCK_CONST_METHOD1(loadProgramList, std::list<ProgramDto>(const std::string& text));
	MOCK_CONST_METHOD1(loadRunTimeList, std::list<RunTimeDto>(const std::string& text));
	MOCK_CONST_METHOD1(loadStartTimeList, std::list<StartTimeDto>(const std::string& text));
};
