#include "Logic/Impl/ProgramImpl.h"
#include "Logic/Impl/SchedulerContainerImpl.h"
#include "Logic/Impl/StartTimeImpl.h"
#include "Logic/Impl/StartTimeContainerImpl.h"
#include "Mocks/MockScheduler.h"
#include "Mocks/MockSchedulerContainer.h"
#include "Mocks/MockRunTimeContainer.h"
#include "Mocks/MockStartTimeContainer.h"
#include "Mocks/MockStartTime.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramImplTest, defaultConstructor) {
	const auto mockSchedulerContainer = std::make_shared<MockSchedulerContainer>();
	const auto mockRunTimeContainer = std::make_shared<MockRunTimeContainer>();
	const auto mockStartTimeContainer = std::make_shared<MockStartTimeContainer>();

	const ProgramImpl program(
		mockSchedulerContainer,
		mockRunTimeContainer,
		mockStartTimeContainer
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

	const ProgramImpl program(
		enabled, name, adjustment, schedulerType,
		mockSchedulerContainer,
		mockRunTimeContainer,
		mockStartTimeContainer
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

	ProgramImpl program(
		std::make_shared<MockSchedulerContainer>(),
		std::make_shared<MockRunTimeContainer>(),
		std::make_shared<MockStartTimeContainer>()
	);

	program.setEnabled(expected1);
	EXPECT_THAT(program.isEnabled(), Eq(expected1));

	program.setEnabled(expected2);
	EXPECT_THAT(program.isEnabled(), Eq(expected2));
}

TEST(ProgramImplTest, setName) {
	const std::string expected1("AbcXyz");
	const std::string expected2("9876543210");

	ProgramImpl program(
		std::make_shared<MockSchedulerContainer>(),
		std::make_shared<MockRunTimeContainer>(),
		std::make_shared<MockStartTimeContainer>()
	);

	program.setName(expected1);
	EXPECT_THAT(program.getName(), Eq(expected1));

	program.setName(expected2);
	EXPECT_THAT(program.getName(), Eq(expected2));
}

TEST(ProgramImplTest, setAdjustment) {
	const unsigned expected1 = 70;
	const unsigned expected2 = 71;

	ProgramImpl program(
		std::make_shared<MockSchedulerContainer>(),
		std::make_shared<MockRunTimeContainer>(),
		std::make_shared<MockStartTimeContainer>()
	);

	program.setAdjustment(expected1);
	EXPECT_THAT(program.getAdjustment(), Eq(expected1));

	program.setAdjustment(expected2);
	EXPECT_THAT(program.getAdjustment(), Eq(expected2));
}

TEST(ProgramImplTest, setSchedulerType) {
	const SchedulerType expected1 = SchedulerType::EVERY_DAY;
	const SchedulerType expected2 = SchedulerType::WEEKLY;

	ProgramImpl program(
		std::make_shared<MockSchedulerContainer>(),
		std::make_shared<MockRunTimeContainer>(),
		std::make_shared<MockStartTimeContainer>()
	);

	program.setSchedulerType(expected1);
	EXPECT_THAT(program.getSchedulerType(), Eq(expected1));

	program.setSchedulerType(expected2);
	EXPECT_THAT(program.getSchedulerType(), Eq(expected2));
}

TEST(ProgramImplTest, isScheduled) {
	const unsigned year = 2018;
	const unsigned month = 5;
	const unsigned day = 27;

	const std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer = std::make_shared<StrictMock<MockSchedulerContainer>>();
	const std::shared_ptr<MockScheduler> mockScheduler = std::make_shared<StrictMock<MockScheduler>>();

	EXPECT_CALL(*mockSchedulerContainer, at(_)).
			WillRepeatedly(Return(mockScheduler));

	for (unsigned sec = 0; sec < 60; sec++) {
		EXPECT_CALL(*mockScheduler, process(LocalDateTime(year, month, day, 4, 0, sec))).
				Times(1).
				WillRepeatedly(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(static_cast<unsigned>(4 * 3600 + 0 * 60 + sec))))));

		EXPECT_CALL(*mockScheduler, process(LocalDateTime(year, month, day, 6, 30, sec))).
				Times(1).
				WillRepeatedly(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(static_cast<unsigned>(6 * 3600 + 30 * 60 + sec))))));

		EXPECT_CALL(*mockScheduler, process(LocalDateTime(year, month, day, 20, 15, sec))).
				Times(1).
				WillRepeatedly(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(static_cast<unsigned>(20 * 3600 + 15 * 60 + sec))))));
	}

	ProgramImpl program(
		mockSchedulerContainer,
		std::make_shared<MockRunTimeContainer>(),
		std::make_shared<StartTimeContainerImpl>(std::make_shared<StartTimeImplFactory>())
	);

	program.getStartTimeContainer().insert(0, std::make_shared<StartTimeImpl>(4, 0));
	program.getStartTimeContainer().insert(1, std::make_shared<StartTimeImpl>(6, 30));
	program.getStartTimeContainer().insert(2, std::make_shared<StartTimeImpl>(20, 15));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {

				bool scheduled = false;
				scheduled |= (hour == 4 && min == 0);
				scheduled |= (hour == 6 && min == 30);
				scheduled |= (hour == 20 && min == 15);

				const LocalDateTime localDateTime(year, month, day, hour, min, sec);

				if (scheduled) {
					ASSERT_THAT(program.isScheduled(localDateTime), Pointee(Scheduler::Result(static_cast<unsigned>(hour * 3600 + min * 60 + sec))));
				} else {
					ASSERT_THAT(program.isScheduled(localDateTime), IsNull());
				}
			}
		}
	}
}

TEST(ProgramImplTest, isScheduled_disabled) {
	std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer = std::make_shared<StrictMock<MockSchedulerContainer>>();

	ProgramImpl program(
		mockSchedulerContainer,
		std::make_shared<MockRunTimeContainer>(),
		std::make_shared<StartTimeContainerImpl>(std::make_shared<StartTimeImplFactory>())
	);

	program.setEnabled(false);
	program.getStartTimeContainer().insert(0, std::make_shared<StartTimeImpl>(4, 0));
	program.getStartTimeContainer().insert(1, std::make_shared<StartTimeImpl>(6, 30));
	program.getStartTimeContainer().insert(2, std::make_shared<StartTimeImpl>(20, 15));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				const LocalDateTime localDateTime(2018, 5, 27, hour, min, sec);
				ASSERT_THAT(program.isScheduled(localDateTime), IsNull());
			}
		}
	}
}

TEST(ProgramImplTest, isScheduled_withCorrectScheduler) {
	const LocalDateTime localDateTime(2018, 5, 17, 4, 0, 0);

	std::vector<SchedulerType> schedulerTypes {
		SchedulerType::EVERY_DAY,
		SchedulerType::HOT_WEATHER,
		SchedulerType::TEMPERATURE_DEPENDENT,
		SchedulerType::WEEKLY
	};

	for (const auto schedulerType : schedulerTypes) {
		std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer = std::make_shared<StrictMock<MockSchedulerContainer>>();
		std::shared_ptr<MockScheduler> mockScheduler = std::make_shared<StrictMock<MockScheduler>>();

		EXPECT_CALL(*mockSchedulerContainer, at(schedulerType)).WillRepeatedly(Return(mockScheduler));

		EXPECT_CALL(*mockScheduler, process(localDateTime)).
				Times(1).
				WillRepeatedly(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(0u)))));

		ProgramImpl program(
			mockSchedulerContainer,
			std::make_shared<MockRunTimeContainer>(),
			std::make_shared<StartTimeContainerImpl>(std::make_shared<StartTimeImplFactory>())
		);

		program.getStartTimeContainer().insert(0, std::make_shared<StartTimeImpl>(4, 0));
		program.setSchedulerType(schedulerType);

		ASSERT_THAT(program.isScheduled(localDateTime), Pointee(Scheduler::Result(0u)));
	}
}
