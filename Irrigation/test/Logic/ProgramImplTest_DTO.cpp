#include "ProgramImplTest.h"
#include "Dto2ObjectSamples/ProgramSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramImplToDtoTest, toProgramDto) {
	const Dto2ObjectTestSamples::ProgramSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const ProgramImpl& actual = sample.getObject();
		const ProgramDTO& expected = sample.getDto();

		EXPECT_THAT(actual.toProgramDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

void ProgramImplFromDtoTest::checkUpdateFromProgramDto(const UpdateType updateType) {
	const auto sampleList = Dto2ObjectTestSamples::ProgramSampleList();

	bool expectedEnabled = program->isEnabled();
	std::string expectedName = program->getName();
	unsigned expectedAdjustment = program->getAdjustment();
	std::string expectedSchedulerType = to_string(program->getSchedulerType());

	for (const auto& sample : sampleList) {
		ProgramDTO actualProgramDTO;

		if (UpdateType::Enabled == updateType || UpdateType::All == updateType) {
			expectedEnabled = sample.getDto().getEnabled();
			actualProgramDTO.setEnabled(expectedEnabled);
		}

		if (UpdateType::Name == updateType || UpdateType::All == updateType) {
			expectedName = sample.getDto().getName();
			actualProgramDTO.setName(expectedName);
		}

		if (UpdateType::Adjustment == updateType || UpdateType::All == updateType) {
			expectedAdjustment = sample.getDto().getAdjustment();
			actualProgramDTO.setAdjustment(expectedAdjustment);
		}

		if (UpdateType::SchedulerType == updateType || UpdateType::All == updateType) {
			expectedSchedulerType = sample.getDto().getSchedulerType();
			actualProgramDTO.setSchedulerType(expectedSchedulerType);
		}

		if (UpdateType::SchedulerContainer == updateType || UpdateType::All == updateType) {
			const SchedulersDTO expectedSchedulersDTO = sample.getDto().getSchedulers();

			actualProgramDTO.setSchedulers(SchedulersDTO(expectedSchedulersDTO));
			EXPECT_CALL(*mockSchedulerContainer, updateFromSchedulersDto(expectedSchedulersDTO));
		}

		if (UpdateType::RunTimeContainer == updateType || UpdateType::All == updateType) {
			const std::list<RunTimeDTO> expectedRunTimeDtoList = sample.getDto().getRunTimes();

			actualProgramDTO.setRunTimes(std::list<RunTimeDTO>(expectedRunTimeDtoList));
			EXPECT_CALL(*mockRunTimeContainer, updateFromRunTimeDtoList(expectedRunTimeDtoList));
		}

		if (UpdateType::StartTimeContainer == updateType || UpdateType::All == updateType) {
			const std::list<StartTimeDTO> expectedStartTimeDtoList = sample.getDto().getStartTimes();

			actualProgramDTO.setStartTimes(std::list<StartTimeDTO>(expectedStartTimeDtoList));
			EXPECT_CALL(*mockStartTimeContainer, updateFromStartTimeDtoList(expectedStartTimeDtoList));
		}

		program->updateFromProgramDto(actualProgramDTO);

		EXPECT_THAT(program->isEnabled(), Eq(expectedEnabled));
		EXPECT_THAT(program->getName(), Eq(expectedName));
		EXPECT_THAT(program->getAdjustment(), Eq(expectedAdjustment));
		EXPECT_THAT(to_string(program->getSchedulerType()), Eq(expectedSchedulerType));
	}
}

TEST_F(ProgramImplFromDtoTest, updateFromProgramDto_empty) {
	program->setEnabled(false);
	program->setName("1234");
	program->setAdjustment(820);
	program->setSchedulerType(SchedulerType::EVERY_DAY);

	checkUpdateFromProgramDto(UpdateType::Nothing);
}

TEST_F(ProgramImplFromDtoTest, updateFromProgramDto_partial_enabled) {
	program->setEnabled(true);
	program->setName("234");
	program->setAdjustment(850);
	program->setSchedulerType(SchedulerType::HOT_WEATHER);

	checkUpdateFromProgramDto(UpdateType::Enabled);
}

TEST_F(ProgramImplFromDtoTest, updateFromProgramDto_partial_name) {
	program->setEnabled(false);
	program->setName("987");
	program->setAdjustment(860);
	program->setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT);

	checkUpdateFromProgramDto(UpdateType::Name);
}

TEST_F(ProgramImplFromDtoTest, updateFromProgramDto_partial_adjustament) {
	program->setEnabled(true);
	program->setName("456");
	program->setAdjustment(890);
	program->setSchedulerType(SchedulerType::WEEKLY);

	checkUpdateFromProgramDto(UpdateType::Adjustment);
}

TEST_F(ProgramImplFromDtoTest, updateFromProgramDto_partial_schedulerType) {
	program->setEnabled(false);
	program->setName("11111");
	program->setAdjustment(720);
	program->setSchedulerType(SchedulerType::EVERY_DAY);

	checkUpdateFromProgramDto(UpdateType::SchedulerType);
}

TEST_F(ProgramImplFromDtoTest, updateFromProgramDto_partial_schedulerContainer) {
	program->setEnabled(true);
	program->setName("abcd");
	program->setAdjustment(770);
	program->setSchedulerType(SchedulerType::HOT_WEATHER);

	checkUpdateFromProgramDto(UpdateType::SchedulerContainer);
}

TEST_F(ProgramImplFromDtoTest, updateFromProgramDto_partial_runTimeContainer) {
	program->setEnabled(false);
	program->setName("ppppp");
	program->setAdjustment(660);
	program->setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT);

	checkUpdateFromProgramDto(UpdateType::RunTimeContainer);
}

TEST_F(ProgramImplFromDtoTest, updateFromProgramDto_partial_startTimeContainer) {
	program->setEnabled(true);
	program->setName("98765");
	program->setAdjustment(316);
	program->setSchedulerType(SchedulerType::WEEKLY);

	checkUpdateFromProgramDto(UpdateType::StartTimeContainer);
}

TEST_F(ProgramImplFromDtoTest, updateFromProgramDto_all) {
	program->setEnabled(false);
	program->setName("8888888");
	program->setAdjustment(999);
	program->setSchedulerType(SchedulerType::EVERY_DAY);

	checkUpdateFromProgramDto(UpdateType::All);
}
