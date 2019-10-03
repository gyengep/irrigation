#pragma once
#include <gmock/gmock.h>
#include <memory>
#include <DtoReaderWriter/DtoReaderWriter.h>
#include "DTO/DocumentDTO.h"


class MockDtoReader : public DtoReader {
public:
	MOCK_CONST_METHOD1(loadDocument, DocumentDTO(const std::string&));
	MOCK_CONST_METHOD1(loadProgram, ProgramDTO(const std::string&));
	MOCK_CONST_METHOD1(loadRunTime, RunTimeDTO(const std::string&));
	MOCK_CONST_METHOD1(loadStartTime, StartTimeDTO(const std::string&));
	MOCK_CONST_METHOD1(loadEveryDayScheduler, EveryDaySchedulerDTO(const std::string&));
	MOCK_CONST_METHOD1(loadHotWeatherScheduler, HotWeatherSchedulerDTO(const std::string&));
	MOCK_CONST_METHOD1(loadPeriodicScheduler, PeriodicSchedulerDTO(const std::string&));
	MOCK_CONST_METHOD1(loadTemperatureDependentScheduler, TemperatureDependentSchedulerDTO(const std::string&));
	MOCK_CONST_METHOD1(loadWeeklyScheduler, WeeklySchedulerDTO(const std::string&));
	MOCK_CONST_METHOD1(loadProgramList, std::list<ProgramDTO>(const std::string& text));
	MOCK_CONST_METHOD1(loadRunTimeList, std::list<RunTimeDTO>(const std::string& text));
	MOCK_CONST_METHOD1(loadStartTimeList, std::list<StartTimeDTO>(const std::string& text));
};

///////////////////////////////////////////////////////////////////////////////

class MockDtoWriter : public DtoWriter {
public:
	MOCK_METHOD1(save, std::string(const DocumentDTO& document));
	MOCK_METHOD2(save, std::string(const ProgramDTO& program, bool includeContainers));
	MOCK_METHOD1(save, std::string(const RunTimeDTO& runTime));
	MOCK_METHOD1(save, std::string(const StartTimeDTO& startTime));
	MOCK_METHOD1(save, std::string(const EveryDaySchedulerDTO& scheduler));
	MOCK_METHOD1(save, std::string(const HotWeatherSchedulerDTO& scheduler));
	MOCK_METHOD1(save, std::string(const PeriodicSchedulerDTO& scheduler));
	MOCK_METHOD1(save, std::string(const TemperatureDependentSchedulerDTO& scheduler));
	MOCK_METHOD1(save, std::string(const WeeklySchedulerDTO& scheduler));
	MOCK_METHOD2(save, std::string(const std::list<ProgramDTO>& programs, bool includeContainers));
	MOCK_METHOD1(save, std::string(const std::list<RunTimeDTO>& runTimes));
	MOCK_METHOD1(save, std::string(const std::list<StartTimeDTO>& startTimes));
};
