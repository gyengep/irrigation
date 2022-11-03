#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Mocks/MockSchedulerContainer.h"
#include "Mocks/MockRunTimeContainer.h"
#include "Mocks/MockStartTime.h"
#include "Mocks/MockStartTimeContainer.h"
#include "Logic/ProgramImpl.h"


class ProgramImplUpdateFromOrToDtoTest : public ::testing::Test {
protected:

	const bool defaultEnabled = false;
	const std::string defaultName = "abcd";
	const unsigned defaultAdjustment = 99;
	const SchedulerType defaultSchedulerType = SchedulerType::EVERY_DAY;

	const bool expectedEnabled1 = true;
	const bool expectedEnabled2 = false;
	const std::string expectedName1 = defaultName + "_ABC";
	const std::string expectedName2 = defaultName + "_123";
	const unsigned expectedAdjustment1 = defaultAdjustment + 1;
	const unsigned expectedAdjustment2 = defaultAdjustment + 2;
	const SchedulerType expectedSchedulerType1 = SchedulerType::WEEKLY;
	const SchedulerType expectedSchedulerType2 = SchedulerType::HOT_WEATHER;
	const SchedulersDTO expectedSchedulersDTO1 = SchedulersDTO(
			EveryDaySchedulerDTO(),
			HotWeatherSchedulerDTO(10, 20),
			TemperatureDependentSchedulerDTO(0, 1, 2, 3),
			WeeklySchedulerDTO(std::list<bool> { true, true, false, false, true, false, false})
		);
	const SchedulersDTO expectedSchedulersDTO2 = SchedulersDTO(
			EveryDaySchedulerDTO(),
			HotWeatherSchedulerDTO(25, 35),
			TemperatureDependentSchedulerDTO(10, 20, 30, 40),
			WeeklySchedulerDTO(std::list<bool> { false, false, true, false, true, false, false})
		);
	const std::list<RunTimeDTO> expectedRunTimeDtoList1 = std::list<RunTimeDTO> { RunTimeDTO(10, 20), RunTimeDTO(30, 40), RunTimeDTO(50, 60) };
	const std::list<RunTimeDTO> expectedRunTimeDtoList2 = std::list<RunTimeDTO> { RunTimeDTO(25, 35), RunTimeDTO(11, 12) };
	const std::list<StartTimeDTO> expectedStartTimeDtoList1 = std::list<StartTimeDTO> { StartTimeDTO(10, 20), StartTimeDTO(50, 60) };
	const std::list<StartTimeDTO> expectedStartTimeDtoList2 = std::list<StartTimeDTO> { StartTimeDTO(25, 35), StartTimeDTO(11, 12), StartTimeDTO(44, 45) };

	const ProgramDTO expectedProgramDto1 {
		expectedEnabled1, expectedName1, expectedAdjustment1, to_string(expectedSchedulerType1),
		SchedulersDTO(expectedSchedulersDTO1),
		std::list<RunTimeDTO>(expectedRunTimeDtoList1),
		std::list<StartTimeDTO>(expectedStartTimeDtoList1)
	};

	const ProgramDTO expectedProgramDto2 {
		expectedEnabled2, expectedName2, expectedAdjustment2, to_string(expectedSchedulerType2),
		SchedulersDTO(expectedSchedulersDTO2),
		std::list<RunTimeDTO>(expectedRunTimeDtoList2),
		std::list<StartTimeDTO>(expectedStartTimeDtoList2)
	};


	std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer;
	std::shared_ptr<MockRunTimeContainer> mockRunTimeContainer;
	std::shared_ptr<MockStartTimeContainer> mockStartTimeContainer;
	std::shared_ptr<StartTimeFactory> mockStartTimeFactory;

	std::shared_ptr<ProgramImpl> program;

	virtual void SetUp();
    virtual void TearDown();
};
