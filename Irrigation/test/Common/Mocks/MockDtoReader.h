#pragma once
#include <gmock/gmock.h>
#include <DtoReaderWriter/DtoReaderWriter.h>


class MockDtoReader : public DtoReader {
public:
	MOCK_CONST_METHOD1(loadDocumentDto, DocumentDto(const std::string&));
	MOCK_CONST_METHOD1(loadProgramDto, ProgramDto(const std::string&));
	MOCK_CONST_METHOD1(loadRunTimeDto, RunTimeDto(const std::string&));
	MOCK_CONST_METHOD1(loadStartTimeDto, StartTimeDto(const std::string&));
	MOCK_CONST_METHOD1(loadSchedulersDto, SchedulersDto(const std::string& text));
	MOCK_CONST_METHOD1(loadEveryDaySchedulerDto, EveryDaySchedulerDto(const std::string&));
	MOCK_CONST_METHOD1(loadHotWeatherSchedulerDto, HotWeatherSchedulerDto(const std::string&));
	MOCK_CONST_METHOD1(loadTemperatureDependentSchedulerDto, TemperatureDependentSchedulerDto(const std::string&));
	MOCK_CONST_METHOD1(loadWeeklySchedulerDto, WeeklySchedulerDto(const std::string&));
	MOCK_CONST_METHOD1(loadProgramDtoList, ProgramDtoList(const std::string& text));
	MOCK_CONST_METHOD1(loadRunTimeDtoList, RunTimeDtoList(const std::string& text));
	MOCK_CONST_METHOD1(loadStartTimeDtoList, StartTimeDtoList(const std::string& text));
};
