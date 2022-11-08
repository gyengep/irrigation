#include "ProgramImplTest.h"
#include "Logic/ProgramImplBuilder.h"
#include "Logic/StartTimeImpl.h"
#include "Utils/TimeConversion.h"

using namespace testing;
using ::testing::Return;
using ::testing::AnyNumber;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramImplTest, defaultConstructor) {
	const auto mockSchedulerContainer = std::make_shared<MockSchedulerContainer>();
	const auto mockRunTimeContainer = std::make_shared<MockRunTimeContainer>();
	const auto mockStartTimeContainer = std::make_shared<MockStartTimeContainer>();
	const auto mockStartTimeFactory = std::make_shared<MockStartTimeFactory>(0);

	const ProgramImpl program(
		mockSchedulerContainer,
		mockRunTimeContainer,
		mockStartTimeContainer,
		mockStartTimeFactory
	);

	EXPECT_TRUE(program.isEnabled());
	EXPECT_THAT(program.getName(), Eq(""));
	EXPECT_THAT(program.getAdjustment(), Eq(100));
	EXPECT_THAT(program.getSchedulerType(), Eq(SchedulerType::WEEKLY));
	EXPECT_THAT(&program.getSchedulerContainer(), Eq(mockSchedulerContainer.get()));
	EXPECT_THAT(&program.getRunTimeContainer(), Eq(mockRunTimeContainer.get()));
	EXPECT_THAT(&program.getStartTimeContainer(), Eq(mockStartTimeContainer.get()));
}

TEST(ProgramImplTest, parametrizedConstructor) {
	const bool enabled(false);
	const std::string name("gcuuzg");
	const unsigned adjustment = 78;
	const SchedulerType schedulerType = SchedulerType::EVERY_DAY;

	const auto mockSchedulerContainer = std::make_shared<MockSchedulerContainer>();
	const auto mockRunTimeContainer = std::make_shared<MockRunTimeContainer>();
	const auto mockStartTimeContainer = std::make_shared<MockStartTimeContainer>();
	const auto mockStartTimeFactory = std::make_shared<MockStartTimeFactory>(0);

	const ProgramImpl program(
		enabled, name, adjustment, schedulerType,
		mockSchedulerContainer,
		mockRunTimeContainer,
		mockStartTimeContainer,
		mockStartTimeFactory
	);

	EXPECT_THAT(program.isEnabled(), Eq(enabled));
	EXPECT_THAT(program.getName(), Eq(name));
	EXPECT_THAT(program.getAdjustment(), Eq(adjustment));
	EXPECT_THAT(program.getSchedulerType(), Eq(schedulerType));
	EXPECT_THAT(&program.getSchedulerContainer(), Eq(mockSchedulerContainer.get()));
	EXPECT_THAT(&program.getRunTimeContainer(), Eq(mockRunTimeContainer.get()));
	EXPECT_THAT(&program.getStartTimeContainer(), Eq(mockStartTimeContainer.get()));
}

TEST(ProgramImplTest, setEnabled) {
	const bool expected1 = true;
	const bool expected2 = false;

	ProgramPtr program = ProgramImpl::Builder().build();

	program->setEnabled(expected1);
	EXPECT_THAT(program->isEnabled(), Eq(expected1));

	program->setEnabled(expected2);
	EXPECT_THAT(program->isEnabled(), Eq(expected2));
}

TEST(ProgramImplTest, setName) {
	const std::string expected1("AbcXyz");
	const std::string expected2("9876543210");

	ProgramPtr program = ProgramImpl::Builder().build();

	program->setName(expected1);
	EXPECT_THAT(program->getName(), Eq(expected1));

	program->setName(expected2);
	EXPECT_THAT(program->getName(), Eq(expected2));
}

TEST(ProgramImplTest, setAdjustment) {
	const unsigned expected1 = 70;
	const unsigned expected2 = 71;

	ProgramPtr program = ProgramImpl::Builder().build();

	program->setAdjustment(expected1);
	EXPECT_THAT(program->getAdjustment(), Eq(expected1));

	program->setAdjustment(expected2);
	EXPECT_THAT(program->getAdjustment(), Eq(expected2));
}

TEST(ProgramImplTest, setSchedulerType) {
	const SchedulerType expected1 = SchedulerType::EVERY_DAY;
	const SchedulerType expected2 = SchedulerType::WEEKLY;

	ProgramPtr program = ProgramImpl::Builder().build();

	program->setSchedulerType(expected1);
	EXPECT_THAT(program->getSchedulerType(), Eq(expected1));

	program->setSchedulerType(expected2);
	EXPECT_THAT(program->getSchedulerType(), Eq(expected2));
}

TEST(ProgramImplTest, isScheduled1) {
	std::shared_ptr<MockScheduler> mockScheduler = std::make_shared<StrictMock<MockScheduler>>();
	std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer = std::make_shared<StrictMock<MockSchedulerContainer>>();

	EXPECT_CALL(*mockScheduler, process(_)).
			Times(4).
			WillRepeatedly(Return(Scheduler::Result(false, false, 0)));

	EXPECT_CALL(*mockSchedulerContainer, at(_)).
			WillRepeatedly(Return(mockScheduler));

	ProgramPtr program = ProgramImpl::Builder().
			setSchedulerContainer(mockSchedulerContainer).
			build();

	program->setAdjustment(76);
	program->getStartTimeContainer().insert(0, std::make_shared<StartTimeImpl>(4, 0));
	program->getStartTimeContainer().insert(1, std::make_shared<StartTimeImpl>(6, 0));
	program->getStartTimeContainer().insert(2, std::make_shared<StartTimeImpl>(6, 30));
	program->getStartTimeContainer().insert(3, std::make_shared<StartTimeImpl>(20, 15));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {

				bool expectedResult = false;
				expectedResult |= (hour == 4 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 30 && sec == 0);
				expectedResult |= (hour == 20 && min == 15 && sec == 0);

				const time_t rawTime = fromLocalTime(2018, 5, 27, hour, min, sec);
				ASSERT_THAT(program->isScheduled(rawTime), Pointee(ScheduledResult(expectedResult, 0)));
			}
		}
	}
}

TEST(ProgramImplTest, isScheduled2) {
	std::shared_ptr<MockScheduler> mockScheduler = std::make_shared<StrictMock<MockScheduler>>();
	std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer = std::make_shared<StrictMock<MockSchedulerContainer>>();

	EXPECT_CALL(*mockScheduler, process(_)).
			Times(4).
			WillRepeatedly(Return(Scheduler::Result(true, false, 0)));

	EXPECT_CALL(*mockSchedulerContainer, at(_)).
			WillRepeatedly(Return(mockScheduler));

	ProgramPtr program = ProgramImpl::Builder().
			setSchedulerContainer(mockSchedulerContainer).
			build();

	program->setAdjustment(76);
	program->getStartTimeContainer().insert(0, std::make_shared<StartTimeImpl>(4, 0));
	program->getStartTimeContainer().insert(1, std::make_shared<StartTimeImpl>(6, 0));
	program->getStartTimeContainer().insert(2, std::make_shared<StartTimeImpl>(6, 30));
	program->getStartTimeContainer().insert(3, std::make_shared<StartTimeImpl>(20, 15));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				bool expectedResult = false;
				expectedResult |= (hour == 4 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 30 && sec == 0);
				expectedResult |= (hour == 20 && min == 15 && sec == 0);

				const time_t rawTime = fromLocalTime(2018, 5, 27, hour, min, sec);
				ASSERT_THAT(program->isScheduled(rawTime), Pointee(ScheduledResult(expectedResult, expectedResult ? 76 : 0)));
			}
		}
	}
}

TEST(ProgramImplTest, isScheduled3) {
	std::shared_ptr<MockScheduler> mockScheduler = std::make_shared<StrictMock<MockScheduler>>();
	std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer = std::make_shared<StrictMock<MockSchedulerContainer>>();

	EXPECT_CALL(*mockScheduler, process(_)).
			Times(4).
			WillRepeatedly(Return(Scheduler::Result(true, true, 25)));

	EXPECT_CALL(*mockSchedulerContainer, at(_)).
			WillRepeatedly(Return(mockScheduler));

	ProgramPtr program = ProgramImpl::Builder().
			setSchedulerContainer(mockSchedulerContainer).
			build();

	program->setAdjustment(76);
	program->getStartTimeContainer().insert(0, std::make_shared<StartTimeImpl>(4, 0));
	program->getStartTimeContainer().insert(1, std::make_shared<StartTimeImpl>(6, 0));
	program->getStartTimeContainer().insert(2, std::make_shared<StartTimeImpl>(6, 30));
	program->getStartTimeContainer().insert(3, std::make_shared<StartTimeImpl>(20, 15));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				bool expectedResult = false;
				expectedResult |= (hour == 4 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 30 && sec == 0);
				expectedResult |= (hour == 20 && min == 15 && sec == 0);

				const time_t rawTime = fromLocalTime(2018, 5, 27, hour, min, sec);
				ASSERT_THAT(program->isScheduled(rawTime), Pointee(ScheduledResult(expectedResult, expectedResult ? 0.25f * 0.76f * 100.0f : 0)));
			}
		}
	}
}

TEST(ProgramImplTest, isScheduled_disabled) {
	std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer = std::make_shared<StrictMock<MockSchedulerContainer>>();

	ProgramPtr program = ProgramImpl::Builder().
			setSchedulerContainer(mockSchedulerContainer).
			build();

	program->setEnabled(false);
	program->getStartTimeContainer().insert(0, std::make_shared<StartTimeImpl>(4, 0));
	program->getStartTimeContainer().insert(1, std::make_shared<StartTimeImpl>(6, 0));
	program->getStartTimeContainer().insert(2, std::make_shared<StartTimeImpl>(6, 30));
	program->getStartTimeContainer().insert(3, std::make_shared<StartTimeImpl>(20, 15));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				const time_t rawTime = fromLocalTime(2018, 5, 27, hour, min, sec);
				ASSERT_THAT(program->isScheduled(rawTime), Pointee(ScheduledResult(false, 0)));
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////

#define CHECK_VALUES(program, enabled, name, adjustment, schedulerType)		\
{																			\
	EXPECT_THAT(program->isEnabled(), Eq(enabled));							\
	EXPECT_THAT(program->getName(), Eq(name));								\
	EXPECT_THAT(program->getAdjustment(), Eq(adjustment));					\
	EXPECT_THAT(program->getSchedulerType(), Eq(schedulerType));			\
}


void ProgramImplUpdateFromOrToDtoTest::SetUp() {
	mockSchedulerContainer = std::make_shared<StrictMock<MockSchedulerContainer>>();
	mockRunTimeContainer = std::make_shared<StrictMock<MockRunTimeContainer>>();
	mockStartTimeContainer = std::make_shared<StrictMock<MockStartTimeContainer>>();
	mockStartTimeFactory = std::make_shared<StrictMock<MockStartTimeFactory>>(0);

	program = std::make_shared<ProgramImpl>(
		defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType,
		mockSchedulerContainer,
		mockRunTimeContainer,
		mockStartTimeContainer,
		mockStartTimeFactory
	);
}

void ProgramImplUpdateFromOrToDtoTest::TearDown() {
}


TEST_F(ProgramImplUpdateFromOrToDtoTest, toDto1) {
	EXPECT_CALL(*mockSchedulerContainer, toSchedulersDto()).Times(1).WillOnce(Return(expectedSchedulersDTO1));
	EXPECT_CALL(*mockRunTimeContainer, toRunTimeDtoList()).Times(1).WillOnce(Return(expectedRunTimeDtoList1));
	EXPECT_CALL(*mockStartTimeContainer, toStartTimeDtoList()).Times(1).WillOnce(Return(expectedStartTimeDtoList1));

	program->setEnabled(expectedEnabled1);
	program->setName(expectedName1);
	program->setAdjustment(expectedAdjustment1);
	program->setSchedulerType(expectedSchedulerType1);

	EXPECT_THAT(program->toProgramDto(), Eq(expectedProgramDto1));
}

TEST_F(ProgramImplUpdateFromOrToDtoTest, toDto2) {
	EXPECT_CALL(*mockSchedulerContainer, toSchedulersDto()).Times(1).WillOnce(Return(expectedSchedulersDTO2));
	EXPECT_CALL(*mockRunTimeContainer, toRunTimeDtoList()).Times(1).WillOnce(Return(expectedRunTimeDtoList2));
	EXPECT_CALL(*mockStartTimeContainer, toStartTimeDtoList()).Times(1).WillOnce(Return(expectedStartTimeDtoList2));

	program->setEnabled(expectedEnabled2);
	program->setName(expectedName2);
	program->setAdjustment(expectedAdjustment2);
	program->setSchedulerType(expectedSchedulerType2);

	EXPECT_THAT(program->toProgramDto(), Eq(expectedProgramDto2));
}


TEST_F(ProgramImplUpdateFromOrToDtoTest, fromDto_empty) {
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO());
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
}

TEST_F(ProgramImplUpdateFromOrToDtoTest, fromDto_partial_Enabled) {
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setEnabled(expectedEnabled1));
	CHECK_VALUES(program, expectedEnabled1, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setEnabled(expectedEnabled2));
	CHECK_VALUES(program, expectedEnabled2, defaultName, defaultAdjustment, defaultSchedulerType);
}

TEST_F(ProgramImplUpdateFromOrToDtoTest, fromDto_partial_Name) {
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setName(expectedName1));
	CHECK_VALUES(program, defaultEnabled, expectedName1, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setName(expectedName2));
	CHECK_VALUES(program, defaultEnabled, expectedName2, defaultAdjustment, defaultSchedulerType);
}

TEST_F(ProgramImplUpdateFromOrToDtoTest, fromDto_partial_Adjustment) {
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setAdjustment(expectedAdjustment1));
	CHECK_VALUES(program, defaultEnabled, defaultName, expectedAdjustment1, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setAdjustment(expectedAdjustment2));
	CHECK_VALUES(program, defaultEnabled, defaultName, expectedAdjustment2, defaultSchedulerType);
}

TEST_F(ProgramImplUpdateFromOrToDtoTest, fromDto_partial_SchedulerType) {
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setSchedulerType(to_string(expectedSchedulerType1)));
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, expectedSchedulerType1);
	program->updateFromProgramDto(ProgramDTO().setSchedulerType(to_string(expectedSchedulerType2)));
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, expectedSchedulerType2);
}

TEST_F(ProgramImplUpdateFromOrToDtoTest, fromDto_partial_SchedulerContainer) {
	EXPECT_CALL(*mockSchedulerContainer, updateFromSchedulersDto(expectedSchedulersDTO1)).Times(1);
	EXPECT_CALL(*mockSchedulerContainer, updateFromSchedulersDto(expectedSchedulersDTO2)).Times(1);

	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setSchedulers(SchedulersDTO(expectedSchedulersDTO1)));
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setSchedulers(SchedulersDTO(expectedSchedulersDTO2)));
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
}

TEST_F(ProgramImplUpdateFromOrToDtoTest, fromDto_partial_RunTimeContainer) {
	EXPECT_CALL(*mockRunTimeContainer, updateFromRunTimeDtoList(expectedRunTimeDtoList1)).Times(1);
	EXPECT_CALL(*mockRunTimeContainer, updateFromRunTimeDtoList(expectedRunTimeDtoList2)).Times(1);

	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setRunTimes(std::list<RunTimeDTO>(expectedRunTimeDtoList1)));
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setRunTimes(std::list<RunTimeDTO>(expectedRunTimeDtoList2)));
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
}

TEST_F(ProgramImplUpdateFromOrToDtoTest, fromDto_partial_StartTimeContainer) {
	EXPECT_CALL(*mockStartTimeContainer, updateFromStartTimeDtoList(mockStartTimeFactory, expectedStartTimeDtoList1)).Times(1);
	EXPECT_CALL(*mockStartTimeContainer, updateFromStartTimeDtoList(mockStartTimeFactory, expectedStartTimeDtoList2)).Times(1);

	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setStartTimes(std::list<StartTimeDTO>(expectedStartTimeDtoList1)));
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(ProgramDTO().setStartTimes(std::list<StartTimeDTO>(expectedStartTimeDtoList2)));
	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
}

TEST_F(ProgramImplUpdateFromOrToDtoTest, fromDto_all) {
	EXPECT_CALL(*mockSchedulerContainer, updateFromSchedulersDto(expectedSchedulersDTO1)).Times(1);
	EXPECT_CALL(*mockSchedulerContainer, updateFromSchedulersDto(expectedSchedulersDTO2)).Times(1);
	EXPECT_CALL(*mockRunTimeContainer, updateFromRunTimeDtoList(expectedRunTimeDtoList1)).Times(1);
	EXPECT_CALL(*mockRunTimeContainer, updateFromRunTimeDtoList(expectedRunTimeDtoList2)).Times(1);
	EXPECT_CALL(*mockStartTimeContainer, updateFromStartTimeDtoList(mockStartTimeFactory, expectedStartTimeDtoList1)).Times(1);
	EXPECT_CALL(*mockStartTimeContainer, updateFromStartTimeDtoList(mockStartTimeFactory, expectedStartTimeDtoList2)).Times(1);

	CHECK_VALUES(program, defaultEnabled, defaultName, defaultAdjustment, defaultSchedulerType);
	program->updateFromProgramDto(expectedProgramDto1);
	CHECK_VALUES(program, expectedEnabled1, expectedName1, expectedAdjustment1, expectedSchedulerType1);
	program->updateFromProgramDto(expectedProgramDto2);
	CHECK_VALUES(program, expectedEnabled2, expectedName2, expectedAdjustment2, expectedSchedulerType2);
}
