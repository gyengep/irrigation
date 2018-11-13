#include <gmock/gmock.h>
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/StartTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Schedulers/WeeklyScheduler.h"
#include "MockProgram.h"
#include "MockScheduler.h"

using namespace std;
using namespace testing;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

///////////////////////////////////////////////////////////////////////////////

TEST(Program, name) {
	Program program;

	EXPECT_TRUE(program.getName().empty());

	const string name("AbcXyz");
	program.setName(name);
	EXPECT_EQ(name, program.getName());
}

TEST(Program, getSchedulerType) {
	Program program;
	EXPECT_EQ(SchedulerType::WEEKLY, program.getSchedulerType());
}

TEST(Program, getWeeklyScheduler) {
	Program program;
	EXPECT_NO_THROW(program.getWeeklyScheduler());
}

TEST(Program, convertProgramDTO) {
	const ProgramDTO expectedProgramDTO("Abcdefg", "weekly",
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
	program.updateFromDTO(expectedProgramDTO);

	EXPECT_THAT(program.getName(), Eq(string("Abcdefg")));
	EXPECT_THAT(program.getSchedulerType(), Eq(SchedulerType::WEEKLY));
	EXPECT_THAT(program.getWeeklyScheduler().getWeeklySchedulerDTO(),
			Eq(expectedProgramDTO.getWeeklyScheduler()));

	EXPECT_THAT(program.getRunTimes().size(), Eq(6));
	EXPECT_THAT(program.getRunTimes().at(0)->getRunTimeDTO().setId(0),
			Eq(*next(expectedProgramDTO.getRunTimes().begin(), 0)));
	EXPECT_THAT(program.getRunTimes().at(1)->getRunTimeDTO().setId(1),
			Eq(*next(expectedProgramDTO.getRunTimes().begin(), 1)));
	EXPECT_THAT(program.getRunTimes().at(2)->getRunTimeDTO().setId(2),
			Eq(*next(expectedProgramDTO.getRunTimes().begin(), 2)));
	EXPECT_THAT(program.getRunTimes().at(3)->getRunTimeDTO().setId(3),
			Eq(*next(expectedProgramDTO.getRunTimes().begin(), 3)));
	EXPECT_THAT(program.getRunTimes().at(4)->getRunTimeDTO().setId(4),
			Eq(*next(expectedProgramDTO.getRunTimes().begin(), 4)));
	EXPECT_THAT(program.getRunTimes().at(5)->getRunTimeDTO().setId(5),
			Eq(*next(expectedProgramDTO.getRunTimes().begin(), 5)));

	EXPECT_THAT(program.getStartTimes().size(), Eq(3));
	EXPECT_THAT(program.getStartTimes().at(100)->getStartTimeDTO().setId(100),
			Eq(*next(expectedProgramDTO.getStartTimes().begin(), 0)));
	EXPECT_THAT(program.getStartTimes().at(101)->getStartTimeDTO().setId(101),
			Eq(*next(expectedProgramDTO.getStartTimes().begin(), 1)));
	EXPECT_THAT(program.getStartTimes().at(102)->getStartTimeDTO().setId(102),
			Eq(*next(expectedProgramDTO.getStartTimes().begin(), 2)));
	EXPECT_THAT(program.getProgramDTO(), Eq(expectedProgramDTO));
}

TEST(Program, updateNameFromProgramDTO) {
	Program program;
	program.setName("abcd");

	program.updateFromDTO(ProgramDTO());
	EXPECT_THAT(program.getName(), Eq(string("abcd")));

	program.updateFromDTO(ProgramDTO().setName("12345"));
	EXPECT_THAT(program.getName(), Eq(string("12345")));
}

TEST(DISABLED_Program, updateSchedulerTypeFromProgramDTO) {
	FAIL();
}

TEST(Program, updateWeeklySchedulerFromProgramDTO) {
	Program program;
	program.getWeeklyScheduler().setAdjustment(73);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::MONDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::TUESDAY, true);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::WEDNESDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::THURSDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::FRIDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::SATURDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::SUNDAY, true);

	program.updateFromDTO(ProgramDTO());
	EXPECT_THAT(program.getWeeklyScheduler().getWeeklySchedulerDTO(),
		Eq(WeeklySchedulerDTO(73, list<bool>({ false, true, false, false, false, false, true }))));

	program.updateFromDTO(ProgramDTO().setWeeklyScheduler(
		WeeklySchedulerDTO(84, list<bool>({ true, false, false, false, true, false, false}))
	));

	EXPECT_THAT(program.getWeeklyScheduler().getWeeklySchedulerDTO(),
		Eq(WeeklySchedulerDTO(84, list<bool>({ true, false, false, false, true, false, false}))));
}

TEST(Program, updateLessWeeklySchedulerFromProgramDTO) {
	Program program;
	program.getWeeklyScheduler().setAdjustment(34);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::SUNDAY, true);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::MONDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::TUESDAY, true);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::WEDNESDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::THURSDAY, true);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::FRIDAY, false);
	program.getWeeklyScheduler().enableDay(WeeklyScheduler::SATURDAY, true);

	program.updateFromDTO(ProgramDTO().setWeeklyScheduler(
		WeeklySchedulerDTO(34, list<bool>({ false, true, false, true }))
	));

	EXPECT_THAT(program.getWeeklyScheduler().getWeeklySchedulerDTO(),
		Eq(WeeklySchedulerDTO(34, list<bool>({ false, true, false, true, false, false, false}))));
}


TEST(Program, updateRunTimesFromProgramDTO) {
	Program program;
	program.getRunTimes().at(0)->setSeconds(60);
	program.getRunTimes().at(1)->setSeconds(61);
	program.getRunTimes().at(2)->setSeconds(62);
	program.getRunTimes().at(3)->setSeconds(63);
	program.getRunTimes().at(4)->setSeconds(64);
	program.getRunTimes().at(5)->setSeconds(65);

	program.updateFromDTO(ProgramDTO());
	EXPECT_THAT(program.getRunTimes().size(), Eq(6));
	EXPECT_THAT(program.getRunTimes().at(0)->getRunTimeDTO(), Eq(RunTimeDTO(1, 0)));
	EXPECT_THAT(program.getRunTimes().at(1)->getRunTimeDTO(), Eq(RunTimeDTO(1, 1)));
	EXPECT_THAT(program.getRunTimes().at(2)->getRunTimeDTO(), Eq(RunTimeDTO(1, 2)));
	EXPECT_THAT(program.getRunTimes().at(3)->getRunTimeDTO(), Eq(RunTimeDTO(1, 3)));
	EXPECT_THAT(program.getRunTimes().at(4)->getRunTimeDTO(), Eq(RunTimeDTO(1, 4)));
	EXPECT_THAT(program.getRunTimes().at(5)->getRunTimeDTO(), Eq(RunTimeDTO(1, 5)));

	program.updateFromDTO(ProgramDTO().setRunTimes(
		list<RunTimeDTO>()
	));

	EXPECT_THAT(program.getRunTimes().size(), Eq(6));
	EXPECT_THAT(program.getRunTimes().at(0)->getRunTimeDTO(), Eq(RunTimeDTO(0, 0)));
	EXPECT_THAT(program.getRunTimes().at(1)->getRunTimeDTO(), Eq(RunTimeDTO(0, 0)));
	EXPECT_THAT(program.getRunTimes().at(2)->getRunTimeDTO(), Eq(RunTimeDTO(0, 0)));
	EXPECT_THAT(program.getRunTimes().at(3)->getRunTimeDTO(), Eq(RunTimeDTO(0, 0)));
	EXPECT_THAT(program.getRunTimes().at(4)->getRunTimeDTO(), Eq(RunTimeDTO(0, 0)));
	EXPECT_THAT(program.getRunTimes().at(5)->getRunTimeDTO(), Eq(RunTimeDTO(0, 0)));
}

TEST(Program, updateRunTimesFromProgramDTOWithoutId) {
	Program program;
	program.updateFromDTO(ProgramDTO().setRunTimes(
		list<RunTimeDTO>({
			RunTimeDTO(2, 0),
			RunTimeDTO(2, 1),
			RunTimeDTO(2, 2),
			RunTimeDTO(2, 3),
			RunTimeDTO(2, 4),
			RunTimeDTO(2, 5)
		})
	));

	EXPECT_THAT(program.getRunTimes().size(), Eq(6));
	EXPECT_THAT(program.getRunTimes().at(0)->getRunTimeDTO(), Eq(RunTimeDTO(2, 0)));
	EXPECT_THAT(program.getRunTimes().at(1)->getRunTimeDTO(), Eq(RunTimeDTO(2, 1)));
	EXPECT_THAT(program.getRunTimes().at(2)->getRunTimeDTO(), Eq(RunTimeDTO(2, 2)));
	EXPECT_THAT(program.getRunTimes().at(3)->getRunTimeDTO(), Eq(RunTimeDTO(2, 3)));
	EXPECT_THAT(program.getRunTimes().at(4)->getRunTimeDTO(), Eq(RunTimeDTO(2, 4)));
	EXPECT_THAT(program.getRunTimes().at(5)->getRunTimeDTO(), Eq(RunTimeDTO(2, 5)));
}

TEST(Program, updateRunTimesFromProgramDTOWithId) {
	Program program;
	program.updateFromDTO(ProgramDTO().setRunTimes(
		list<RunTimeDTO>({
		RunTimeDTO(2, 3).setId(3),
			RunTimeDTO(2, 1).setId(1),
			RunTimeDTO(2, 0).setId(0),
			RunTimeDTO(2, 2).setId(2),
			RunTimeDTO(2, 5).setId(5),
			RunTimeDTO(2, 4).setId(4)
		})
	));

	EXPECT_THAT(program.getRunTimes().size(), Eq(6));
	EXPECT_THAT(program.getRunTimes().at(0)->getRunTimeDTO(), Eq(RunTimeDTO(2, 0)));
	EXPECT_THAT(program.getRunTimes().at(1)->getRunTimeDTO(), Eq(RunTimeDTO(2, 1)));
	EXPECT_THAT(program.getRunTimes().at(2)->getRunTimeDTO(), Eq(RunTimeDTO(2, 2)));
	EXPECT_THAT(program.getRunTimes().at(3)->getRunTimeDTO(), Eq(RunTimeDTO(2, 3)));
	EXPECT_THAT(program.getRunTimes().at(4)->getRunTimeDTO(), Eq(RunTimeDTO(2, 4)));
	EXPECT_THAT(program.getRunTimes().at(5)->getRunTimeDTO(), Eq(RunTimeDTO(2, 5)));
}

TEST(Program, updateRunTimesFromDTOWithAndWithoutId) {
	Program program;
	program.updateFromDTO(ProgramDTO().setRunTimes(
		list<RunTimeDTO>({
			RunTimeDTO(3, 53).setId(3),
			RunTimeDTO(3, 54),
			RunTimeDTO(3, 51).setId(1)
		})
	));

	EXPECT_THAT(program.getRunTimes().size(), Eq(6));
	EXPECT_THAT(program.getRunTimes().at(0)->getRunTimeDTO(), Eq(RunTimeDTO(0, 0)));
	EXPECT_THAT(program.getRunTimes().at(1)->getRunTimeDTO(), Eq(RunTimeDTO(3, 51)));
	EXPECT_THAT(program.getRunTimes().at(2)->getRunTimeDTO(), Eq(RunTimeDTO(0, 0)));
	EXPECT_THAT(program.getRunTimes().at(3)->getRunTimeDTO(), Eq(RunTimeDTO(3, 53)));
	EXPECT_THAT(program.getRunTimes().at(4)->getRunTimeDTO(), Eq(RunTimeDTO(3, 54)));
	EXPECT_THAT(program.getRunTimes().at(5)->getRunTimeDTO(), Eq(RunTimeDTO(0, 0)));
}

TEST(Program, updateStartTimesFromProgramDTO) {
	Program program;
	program.getStartTimes().insert(100, unique_ptr<StartTime>(new StartTime(1, 0)));
	program.getStartTimes().insert(101, unique_ptr<StartTime>(new StartTime(1, 1)));
	program.getStartTimes().insert(102, unique_ptr<StartTime>(new StartTime(1, 2)));

	program.updateFromDTO(ProgramDTO());
	EXPECT_THAT(program.getStartTimes().size(), Eq(3));
	EXPECT_THAT(program.getStartTimes().at(100)->getStartTimeDTO(), Eq(StartTimeDTO(1, 0)));
	EXPECT_THAT(program.getStartTimes().at(101)->getStartTimeDTO(), Eq(StartTimeDTO(1, 1)));
	EXPECT_THAT(program.getStartTimes().at(102)->getStartTimeDTO(), Eq(StartTimeDTO(1, 2)));

	program.updateFromDTO(ProgramDTO().setStartTimes(
		list<StartTimeDTO>({
			StartTimeDTO(2, 0).setId(200),
			StartTimeDTO(2, 1).setId(201),
			StartTimeDTO(2, 2).setId(202)
		})
	));

	EXPECT_THAT(program.getStartTimes().size(), Eq(3));
	EXPECT_THAT(program.getStartTimes().at(200)->getStartTimeDTO(), Eq(StartTimeDTO(2, 0)));
	EXPECT_THAT(program.getStartTimes().at(201)->getStartTimeDTO(), Eq(StartTimeDTO(2, 1)));
	EXPECT_THAT(program.getStartTimes().at(202)->getStartTimeDTO(), Eq(StartTimeDTO(2, 2)));
}


time_t toTime(int year, int month, int day, int hour, int min, int sec, bool dst) {

	EXPECT_TRUE(0 <= month && month < 12);
	EXPECT_TRUE(1 <= day && day < 32);
	EXPECT_TRUE(0 <= hour && hour < 24);
	EXPECT_TRUE(0 <= min && min < 60);
	EXPECT_TRUE(0 <= sec && sec < 60);

	tm tm;

	memset(&tm, 0, sizeof(tm));

	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = min;
	tm.tm_sec = sec;
	tm.tm_isdst = dst ? 1 : 0;

	return mktime(&tm);
}

TEST(Program, isScheduled1) {
	NiceMock<MockProgram_Scheduler> program;
	NiceMock<MockScheduler> scheduler;

	ON_CALL(program, getCurrentScheduler()).WillByDefault(ReturnRef(scheduler));
	ON_CALL(scheduler, isDayScheduled(_)).WillByDefault(Return(false));

	program.getStartTimes().insert(0, unique_ptr<StartTime>(new StartTime(4, 0, 0)));
	program.getStartTimes().insert(1, unique_ptr<StartTime>(new StartTime(6, 0, 0)));
	program.getStartTimes().insert(2, unique_ptr<StartTime>(new StartTime(6, 30, 0)));
	program.getStartTimes().insert(3, unique_ptr<StartTime>(new StartTime(20, 15, 0)));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				time_t t = toTime(2018, 5, 27, hour, min, sec, true);
				EXPECT_FALSE(program.isScheduled(t));
			}
		}
	}
}

TEST(Program, isScheduled2) {
	NiceMock<MockProgram_Scheduler> program;
	NiceMock<MockScheduler> scheduler;

	ON_CALL(program, getCurrentScheduler()).WillByDefault(ReturnRef(scheduler));
	ON_CALL(scheduler, isDayScheduled(_)).WillByDefault(Return(true));

	program.getStartTimes().insert(0, unique_ptr<StartTime>(new StartTime(4, 0, 0)));
	program.getStartTimes().insert(1, unique_ptr<StartTime>(new StartTime(6, 0, 0)));
	program.getStartTimes().insert(2, unique_ptr<StartTime>(new StartTime(6, 30, 0)));
	program.getStartTimes().insert(3, unique_ptr<StartTime>(new StartTime(20, 15, 0)));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				time_t t = toTime(2018, 5, 27, hour, min, sec, true);

				bool requestedResult = false;
				requestedResult |= (hour == 4 && min == 0 && sec == 0);
				requestedResult |= (hour == 6 && min == 0 && sec == 0);
				requestedResult |= (hour == 6 && min == 30 && sec == 0);
				requestedResult |= (hour == 20 && min == 15 && sec == 0);

				EXPECT_EQ(requestedResult, program.isScheduled(t));
			}
		}
	}
}
