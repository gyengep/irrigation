#include "ProgramImplTest.h"
#include "Logic/Impl/StartTimeImpl.h"
#include "Mocks/MockScheduler.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const StartTimeContainer::container_type ProgramImplScheduledTest::startTimeList {
	{ IdType(0), std::make_shared<StartTimeImpl>(4, 0) },
	{ IdType(1), std::make_shared<StartTimeImpl>(6, 30) },
	{ IdType(2), std::make_shared<StartTimeImpl>(20, 15) },
};

///////////////////////////////////////////////////////////////////////////////

void ProgramImplScheduledTest::SetUp() {
	ProgramImplTest::SetUp();

	EXPECT_CALL(*mockStartTimeContainer, begin()).WillRepeatedly(Return(startTimeList.begin()));
	EXPECT_CALL(*mockStartTimeContainer, end()).WillRepeatedly(Return(startTimeList.end()));
}

void ProgramImplScheduledTest::TearDown() {
}

void ProgramImplScheduledTest::checkAppropriateScheduler(const SchedulerType schedulerType) {
	const std::shared_ptr<MockScheduler> mockScheduler = std::make_shared<StrictMock<MockScheduler>>();

	EXPECT_CALL(*mockSchedulerContainer, at(schedulerType)).
			WillRepeatedly(Return(mockScheduler));

	program->setSchedulerType(schedulerType);

	const LocalDateTime localDateTime(2018, 5, 17, 4, 0, 0);

	EXPECT_CALL(*mockScheduler, process(localDateTime)).
			Times(1).
			WillRepeatedly(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(5u)))));

	EXPECT_THAT(program->isScheduled(localDateTime), Pointee(Scheduler::Result(5u)));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramImplScheduledTest, isScheduled) {
	const std::shared_ptr<MockScheduler> mockScheduler = std::make_shared<StrictMock<MockScheduler>>();

	EXPECT_CALL(*mockSchedulerContainer, at(_)).
			WillRepeatedly(Return(mockScheduler));

	program->setEnabled(true);

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

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				const LocalDateTime localDateTime(year, month, day, hour, min, sec);

				bool scheduled = false;
				scheduled |= (hour == 4 && min == 0);
				scheduled |= (hour == 6 && min == 30);
				scheduled |= (hour == 20 && min == 15);

				if (scheduled) {
					ASSERT_THAT(program->isScheduled(localDateTime), Pointee(Scheduler::Result(static_cast<unsigned>(hour * 3600 + min * 60 + sec))));
				} else {
					ASSERT_THAT(program->isScheduled(localDateTime), IsNull());
				}
			}
		}
	}
}

TEST_F(ProgramImplScheduledTest, isScheduled_disabled) {
	const std::shared_ptr<MockScheduler> mockScheduler = std::make_shared<StrictMock<MockScheduler>>();

	EXPECT_CALL(*mockSchedulerContainer, at(_)).
			WillRepeatedly(Return(mockScheduler));

	program->setEnabled(false);

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				const LocalDateTime localDateTime(year, month, day, hour, min, sec);
				ASSERT_THAT(program->isScheduled(localDateTime), IsNull());
			}
		}
	}
}

TEST_F(ProgramImplScheduledTest, isScheduled_withAppropriateScheduler_EveryDay) {
	checkAppropriateScheduler(SchedulerType::EVERY_DAY);
}

TEST_F(ProgramImplScheduledTest, isScheduled_withAppropriateScheduler_HotWeather) {
	checkAppropriateScheduler(SchedulerType::HOT_WEATHER);
}

TEST_F(ProgramImplScheduledTest, isScheduled_withAppropriateScheduler_TemperatureDependemt) {
	checkAppropriateScheduler(SchedulerType::TEMPERATURE_DEPENDENT);
}

TEST_F(ProgramImplScheduledTest, isScheduled_withAppropriateScheduler_Weekly) {
	checkAppropriateScheduler(SchedulerType::WEEKLY);
}
