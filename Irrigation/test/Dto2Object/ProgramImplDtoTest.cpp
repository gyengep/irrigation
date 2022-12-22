#include "Logic/ProgramImpl.h"
#include "Samples/ProgramSamples.h"
#include "Mocks/MockSchedulerContainer.h"
#include "Mocks/MockRunTimeContainer.h"
#include "Mocks/MockStartTimeContainer.h"
#include <gmock/gmock.h>
#include <memory>

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

enum class ProgramDtoUpdateType {
	Nothing,
	Enabled,
	Name,
	Adjustment,
	SchedulerType,
	SchedulerContainer,
	RunTimeContainer,
	StartTimeContainer,
	All
};

void check(const ProgramDtoUpdateType updateType, bool enabled, const std::string& name, unsigned adjustment, SchedulerType schedulerType) {

	std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer = std::make_shared<StrictMock<MockSchedulerContainer>>();
	std::shared_ptr<MockRunTimeContainer> mockRunTimeContainer = std::make_shared<StrictMock<MockRunTimeContainer>>();
	std::shared_ptr<MockStartTimeContainer> mockStartTimeContainer = std::make_shared<StrictMock<MockStartTimeContainer>>();

	ProgramImpl actualProgram(
		enabled, name, adjustment, schedulerType,
		mockSchedulerContainer,
		mockRunTimeContainer,
		mockStartTimeContainer
	);

	bool expectedEnabled = enabled;
	std::string expectedName = name;
	unsigned expectedAdjustment = adjustment;
	std::string expectedSchedulerType = to_string(schedulerType);

	for (const auto& sample : Dto2ObjectTestSamples::ProgramSampleList()) {
		ProgramDTO actualProgramDTO;

		if (ProgramDtoUpdateType::Enabled == updateType || ProgramDtoUpdateType::All == updateType) {
			expectedEnabled = sample.getDto().getEnabled();
			actualProgramDTO.setEnabled(expectedEnabled);
		}

		if (ProgramDtoUpdateType::Name == updateType || ProgramDtoUpdateType::All == updateType) {
			expectedName = sample.getDto().getName();
			actualProgramDTO.setName(expectedName);
		}

		if (ProgramDtoUpdateType::Adjustment == updateType || ProgramDtoUpdateType::All == updateType) {
			expectedAdjustment = sample.getDto().getAdjustment();
			actualProgramDTO.setAdjustment(expectedAdjustment);
		}

		if (ProgramDtoUpdateType::SchedulerType == updateType || ProgramDtoUpdateType::All == updateType) {
			expectedSchedulerType = sample.getDto().getSchedulerType();
			actualProgramDTO.setSchedulerType(expectedSchedulerType);
		}

		if (ProgramDtoUpdateType::SchedulerContainer == updateType || ProgramDtoUpdateType::All == updateType) {
			actualProgramDTO.setSchedulers(SchedulersDTO(sample.getDto().getSchedulers()));
			EXPECT_CALL(*mockSchedulerContainer, updateFromSchedulersDto(sample.getObject().getSchedulerContainer().toSchedulersDto()));
		}

		if (ProgramDtoUpdateType::RunTimeContainer == updateType || ProgramDtoUpdateType::All == updateType) {
			actualProgramDTO.setRunTimes(std::list<RunTimeDTO>(sample.getDto().getRunTimes()));
			EXPECT_CALL(*mockRunTimeContainer, updateFromRunTimeDtoList(sample.getObject().getRunTimeContainer().toRunTimeDtoList()));
		}

		if (ProgramDtoUpdateType::StartTimeContainer == updateType || ProgramDtoUpdateType::All == updateType) {
			actualProgramDTO.setStartTimes(std::list<StartTimeDTO>(sample.getDto().getStartTimes()));
			EXPECT_CALL(*mockStartTimeContainer, updateFromStartTimeDtoList(sample.getObject().getStartTimeContainer().toStartTimeDtoList()));
		}

		actualProgram.updateFromProgramDto(actualProgramDTO);

		EXPECT_THAT(actualProgram.isEnabled(), Eq(expectedEnabled));
		EXPECT_THAT(actualProgram.getName(), Eq(expectedName));
		EXPECT_THAT(actualProgram.getAdjustment(), Eq(expectedAdjustment));
		EXPECT_THAT(to_string(actualProgram.getSchedulerType()), Eq(expectedSchedulerType));
	}
}

TEST(ProgramImplFromDtoTest, updateFromProgramDto_empty) {
	for (const auto& sample : Dto2ObjectTestSamples::ProgramSampleList()) {
		const ProgramImpl& actual = sample.getObject();

		check(ProgramDtoUpdateType::Nothing,
				actual.isEnabled(),
				actual.getName(),
				actual.getAdjustment(),
				actual.getSchedulerType()
			);
	}
}

TEST(ProgramImplFromDtoTest, updateFromProgramDto_partial_enabled) {
	for (const auto& sample : Dto2ObjectTestSamples::ProgramSampleList()) {
		const ProgramImpl& actual = sample.getObject();

		check(ProgramDtoUpdateType::Enabled,
				actual.isEnabled(),
				actual.getName(),
				actual.getAdjustment(),
				actual.getSchedulerType()
			);
	}
}

TEST(ProgramImplFromDtoTest, updateFromProgramDto_partial_name) {
	for (const auto& sample : Dto2ObjectTestSamples::ProgramSampleList()) {
		const ProgramImpl& actual = sample.getObject();

		check(ProgramDtoUpdateType::Name,
				actual.isEnabled(),
				actual.getName(),
				actual.getAdjustment(),
				actual.getSchedulerType()
			);
	}
}

TEST(ProgramImplFromDtoTest, updateFromProgramDto_partial_adjustament) {
	for (const auto& sample : Dto2ObjectTestSamples::ProgramSampleList()) {
		const ProgramImpl& actual = sample.getObject();

		check(ProgramDtoUpdateType::Adjustment,
				actual.isEnabled(),
				actual.getName(),
				actual.getAdjustment(),
				actual.getSchedulerType()
			);
	}
}

TEST(ProgramImplFromDtoTest, updateFromProgramDto_partial_schedulerType) {
	for (const auto& sample : Dto2ObjectTestSamples::ProgramSampleList()) {
		const ProgramImpl& actual = sample.getObject();

		check(ProgramDtoUpdateType::SchedulerType,
				actual.isEnabled(),
				actual.getName(),
				actual.getAdjustment(),
				actual.getSchedulerType()
			);
	}
}

TEST(ProgramImplFromDtoTest, updateFromProgramDto_partial_schedulerContainer) {
	for (const auto& sample : Dto2ObjectTestSamples::ProgramSampleList()) {
		const ProgramImpl& actual = sample.getObject();

		check(ProgramDtoUpdateType::SchedulerContainer,
				actual.isEnabled(),
				actual.getName(),
				actual.getAdjustment(),
				actual.getSchedulerType()
			);
	}
}

TEST(ProgramImplFromDtoTest, updateFromProgramDto_partial_runTimeContainer) {
	for (const auto& sample : Dto2ObjectTestSamples::ProgramSampleList()) {
		const ProgramImpl& actual = sample.getObject();

		check(ProgramDtoUpdateType::RunTimeContainer,
				actual.isEnabled(),
				actual.getName(),
				actual.getAdjustment(),
				actual.getSchedulerType()
			);
	}
}

TEST(ProgramImplFromDtoTest, updateFromProgramDto_partial_startTimeContainer) {
	for (const auto& sample : Dto2ObjectTestSamples::ProgramSampleList()) {
		const ProgramImpl& actual = sample.getObject();

		check(ProgramDtoUpdateType::StartTimeContainer,
				actual.isEnabled(),
				actual.getName(),
				actual.getAdjustment(),
				actual.getSchedulerType()
			);
	}
}

TEST(ProgramImplFromDtoTest, updateFromProgramDto_all) {
	for (const auto& sample : Dto2ObjectTestSamples::ProgramSampleList()) {
		const ProgramImpl& actual = sample.getObject();

		check(ProgramDtoUpdateType::All,
				actual.isEnabled(),
				actual.getName(),
				actual.getAdjustment(),
				actual.getSchedulerType()
			);
	}
}
