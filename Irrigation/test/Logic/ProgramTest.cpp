#include <gmock/gmock.h>
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/StartTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Schedulers/PeriodicScheduler.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Utils/TimeConversion.h"
#include "MockProgram.h"
#include "MockScheduler.h"

using namespace std;
using namespace testing;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramTest, defaultConstructor) {
	Program program;

	EXPECT_TRUE(program.getName().empty());
	EXPECT_THAT(program.getSchedulerType(), Eq(SchedulerType::WEEKLY));
	EXPECT_NO_THROW(program.getPeriodicScheduler());
	EXPECT_NO_THROW(program.getWeeklyScheduler());
}

TEST(ProgramTest, setName) {
	const string name("AbcXyz");

	Program program;
	program.setName(name);
	EXPECT_THAT(program.getName(), Eq(name));
}

TEST(ProgramTest, setSchedulerType) {
	Program program;
	program.setSchedulerType(SchedulerType::PERIODIC);
	EXPECT_THAT(program.getSchedulerType(), Eq(SchedulerType::PERIODIC));
}

TEST(ProgramTest, isScheduled1) {
	NiceMock<MockProgram_Scheduler> program;
	NiceMock<MockScheduler> scheduler;

	ON_CALL(program, getCurrentScheduler()).WillByDefault(ReturnRef(scheduler));
	ON_CALL(scheduler, isDayScheduled(_)).WillByDefault(Return(false));

	program.getStartTimes().insert(0, shared_ptr<StartTime>(new StartTime(4, 0)));
	program.getStartTimes().insert(1, shared_ptr<StartTime>(new StartTime(6, 0)));
	program.getStartTimes().insert(2, shared_ptr<StartTime>(new StartTime(6, 30)));
	program.getStartTimes().insert(3, shared_ptr<StartTime>(new StartTime(20, 15)));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				tm timeinfo = toCalendarTime(2018, 5, 27, hour, min, sec);
				EXPECT_FALSE(program.isScheduled(timeinfo));
			}
		}
	}
}

TEST(ProgramTest, isScheduled2) {
	NiceMock<MockProgram_Scheduler> program;
	NiceMock<MockScheduler> scheduler;

	ON_CALL(program, getCurrentScheduler()).WillByDefault(ReturnRef(scheduler));
	ON_CALL(scheduler, isDayScheduled(_)).WillByDefault(Return(true));

	program.getStartTimes().insert(0, shared_ptr<StartTime>(new StartTime(4, 0)));
	program.getStartTimes().insert(1, shared_ptr<StartTime>(new StartTime(6, 0)));
	program.getStartTimes().insert(2, shared_ptr<StartTime>(new StartTime(6, 30)));
	program.getStartTimes().insert(3, shared_ptr<StartTime>(new StartTime(20, 15)));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				tm timeinfo = toCalendarTime(2018, 5, 27, hour, min, sec);

				bool expectedResult = false;
				expectedResult |= (hour == 4 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 30 && sec == 0);
				expectedResult |= (hour == 20 && min == 15 && sec == 0);

				EXPECT_THAT(program.isScheduled(timeinfo), Eq(expectedResult));
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/*
TEST(ProgramTest, updateNameFromProgramDTO) {
	Program program;
	program.setName("abcd");

	program.updateFromProgramDto(ProgramDTO());
	EXPECT_THAT(program.getName(), Eq(string("abcd")));

	program.updateFromProgramDto(ProgramDTO().setName("12345"));
	EXPECT_THAT(program.getName(), Eq(string("12345")));
}

TEST(ProgramTest, updateSchedulerTypeFromProgramDTO) {
	Program program;
	program.setSchedulerType(SchedulerType::WEEKLY);

	program.updateFromProgramDto(ProgramDTO());
	EXPECT_THAT(program.getSchedulerType(), Eq(SchedulerType::WEEKLY));

	program.updateFromProgramDto(ProgramDTO().setSchedulerType("periodic"));
	EXPECT_THAT(program.getSchedulerType(), Eq(SchedulerType::PERIODIC));

	program.updateFromProgramDto(ProgramDTO().setSchedulerType("weekly"));
	EXPECT_THAT(program.getSchedulerType(), Eq(SchedulerType::WEEKLY));

	EXPECT_THROW(program.updateFromProgramDto(ProgramDTO().setSchedulerType("invalid")), invalid_argument);
}

TEST(ProgramTest, updateWeeklySchedulerFromProgramDTO) {
	Program program;
	program.getWeeklyScheduler().setAdjustment(73);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::MONDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::TUESDAY, true);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::WEDNESDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::THURSDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::FRIDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::SATURDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::SUNDAY, true);

	program.updateFromProgramDto(ProgramDTO());
	EXPECT_THAT(program.getWeeklyScheduler().getWeeklySchedulerDTO(),
		Eq(WeeklySchedulerDTO(73, list<bool>({ false, true, false, false, false, false, true }))));

	program.updateFromProgramDto(ProgramDTO().setWeeklyScheduler(
		WeeklySchedulerDTO(84, list<bool>({ true, false, false, false, true, false, false}))
	));

	EXPECT_THAT(program.getWeeklyScheduler().getWeeklySchedulerDTO(),
		Eq(WeeklySchedulerDTO(84, list<bool>({ true, false, false, false, true, false, false}))));
}

TEST(ProgramTest, updateRunTimesFromProgramDTO) {
	Program program;
	program.getStartTimes().insert(100, shared_ptr<StartTime>(new StartTime(1, 0)));
	program.getStartTimes().insert(101, shared_ptr<StartTime>(new StartTime(1, 1)));
	program.getStartTimes().insert(102, shared_ptr<StartTime>(new StartTime(1, 2)));

	program.updateFromProgramDto(ProgramDTO());
	EXPECT_THAT(program.getStartTimes(), SizeIs(3));
	EXPECT_THAT(program.getStartTimes().at(100)->toStartTimeDto(), Eq(StartTimeDTO(1, 0)));
	EXPECT_THAT(program.getStartTimes().at(101)->toStartTimeDto(), Eq(StartTimeDTO(1, 1)));
	EXPECT_THAT(program.getStartTimes().at(102)->toStartTimeDto(), Eq(StartTimeDTO(1, 2)));

	program.updateFromProgramDto(ProgramDTO().setStartTimes(
		list<StartTimeDTO>({
			StartTimeDTO(2, 0).setId(200),
			StartTimeDTO(2, 1).setId(201)
		})
	));

	EXPECT_THAT(program.getStartTimes(), SizeIs(2));
	EXPECT_THAT(program.getStartTimes().at(200)->toStartTimeDto(), Eq(StartTimeDTO(2, 0)));
	EXPECT_THAT(program.getStartTimes().at(201)->toStartTimeDto(), Eq(StartTimeDTO(2, 1)));
}

TEST(ProgramTest, updateStartTimesFromProgramDTO) {
	Program program;
	program.getStartTimes().insert(100, shared_ptr<StartTime>(new StartTime(1, 0)));
	program.getStartTimes().insert(101, shared_ptr<StartTime>(new StartTime(1, 1)));
	program.getStartTimes().insert(102, shared_ptr<StartTime>(new StartTime(1, 2)));

	program.updateFromProgramDto(ProgramDTO());
	EXPECT_THAT(program.getStartTimes(), SizeIs(3));
	EXPECT_THAT(program.getStartTimes().at(100)->toStartTimeDto(), Eq(StartTimeDTO(1, 0)));
	EXPECT_THAT(program.getStartTimes().at(101)->toStartTimeDto(), Eq(StartTimeDTO(1, 1)));
	EXPECT_THAT(program.getStartTimes().at(102)->toStartTimeDto(), Eq(StartTimeDTO(1, 2)));

	program.updateFromProgramDto(ProgramDTO().setStartTimes(
		list<StartTimeDTO>({
			StartTimeDTO(2, 0).setId(200),
			StartTimeDTO(2, 1).setId(201)
		})
	));

	EXPECT_THAT(program.getStartTimes(), SizeIs(2));
	EXPECT_THAT(program.getStartTimes().at(200)->toStartTimeDto(), Eq(StartTimeDTO(2, 0)));
	EXPECT_THAT(program.getStartTimes().at(201)->toStartTimeDto(), Eq(StartTimeDTO(2, 1)));
}

TEST(ProgramTest, toProgramDto) {
	const ProgramDTO expectedProgramDTO("Abcdefg", "weekly",
			PeriodicSchedulerDTO(55, list<bool>({ false, true, false}), 1978, 9, 29),
			WeeklySchedulerDTO(95, list<bool>({ false, true, false, false, false, true, false})),
			list<RunTimeDTO>({
				RunTimeDTO(20, 30).setId(0),
				RunTimeDTO(21, 31).setId(1),
				RunTimeDTO(22, 32).setId(2),
				RunTimeDTO(23, 33).setId(3),
				RunTimeDTO(24, 34).setId(4),
				RunTimeDTO(25, 35).setId(5)
			}),
			list<StartTimeDTO>({
				StartTimeDTO(10, 20).setId(100),
				StartTimeDTO(11, 21).setId(101),
				StartTimeDTO(12, 22).setId(102)
			})
		);

	Program program;
	program.updateFromProgramDto(expectedProgramDTO);

	EXPECT_THAT(program.toProgramDto(), Eq(expectedProgramDTO));
}
*/
