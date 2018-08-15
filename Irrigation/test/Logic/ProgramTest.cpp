#include <gmock/gmock.h>
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/StartTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Schedulers/SpecifiedScheduler.h"

using namespace std;
using namespace testing;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Return;


TEST(Program, name) {
	Program program;

	EXPECT_TRUE(program.getName().empty());

	const char* name = "AbcXyz";
	program.setName(name);
	EXPECT_EQ(name, program.getName());
}

TEST(Program, getSchedulerType) {
	Program program;
	EXPECT_EQ(SchedulerType::SPECIFIED_DAYS, program.getSchedulerType());
}

TEST(Program, getSpecifiedScheduler) {
	Program program;
	EXPECT_NE(nullptr, &program.getSpecifiedScheduler());
}

TEST(Program, convertProgramDTO) {
	const ProgramDTO expectedProgramDTO("Abcdefg", "specified",
			SpecifiedSchedulerDTO(95, new list<bool>({ false, true, false, false, false, true, false})),
			new list<RunTimeDTO>({
				RunTimeDTO(20, 30).setId(0),
				RunTimeDTO(21, 31).setId(1),
				RunTimeDTO(22, 32).setId(2),
				RunTimeDTO(23, 33).setId(3),
				RunTimeDTO(24, 34).setId(4),
				RunTimeDTO(25, 35).setId(5)
			}),
			new list<StartTimeDTO>({
				StartTimeDTO(10, 20).setId(100),
				StartTimeDTO(11, 21).setId(101),
				StartTimeDTO(12, 22).setId(102)
			})
		);

	Program program;
	program.updateFromDTO(expectedProgramDTO);

	EXPECT_THAT(program.getName(), Eq(string("Abcdefg")));
	EXPECT_THAT(program.getSchedulerType(), Eq(SchedulerType::SPECIFIED_DAYS));
	EXPECT_THAT(program.getSpecifiedScheduler().getSpecifiedSchedulerDTO(),
			Eq(expectedProgramDTO.getSpecifiedScheduler()));

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

TEST(Program, updateSpecifiedSchedulerFromProgramDTO) {
	Program program;
	program.getSpecifiedScheduler().setAdjustment(73);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::SUNDAY, true);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::MONDAY, false);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::TUESDAY, true);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::WEDNESDAY, false);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::THURSDAY, false);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::FRIDAY, false);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::SATURDAY, false);

	program.updateFromDTO(ProgramDTO());
	EXPECT_THAT(program.getSpecifiedScheduler().getSpecifiedSchedulerDTO(),
		Eq(SpecifiedSchedulerDTO(73, new list<bool>({ true, false, true, false, false, false, false }))));

	program.updateFromDTO(ProgramDTO().setSpecifiedScheduler(
		SpecifiedSchedulerDTO(84, new list<bool>({ false, true, false, false, false, true, false}))
	));

	EXPECT_THAT(program.getSpecifiedScheduler().getSpecifiedSchedulerDTO(),
		Eq(SpecifiedSchedulerDTO(84, new list<bool>({ false, true, false, false, false, true, false}))));
}

TEST(Program, updateLessSpecifiedSchedulerFromProgramDTO) {
	Program program;
	program.getSpecifiedScheduler().setAdjustment(34);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::SUNDAY, true);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::MONDAY, false);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::TUESDAY, true);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::WEDNESDAY, false);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::THURSDAY, true);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::FRIDAY, false);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::SATURDAY, true);

	program.updateFromDTO(ProgramDTO().setSpecifiedScheduler(
		SpecifiedSchedulerDTO(34, new list<bool>({ false, true, false, true }))
	));

	EXPECT_THAT(program.getSpecifiedScheduler().getSpecifiedSchedulerDTO(),
		Eq(SpecifiedSchedulerDTO(34, new list<bool>({ false, true, false, true, false, false, false}))));
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
		new list<RunTimeDTO>()
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
		new list<RunTimeDTO>({
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
		new list<RunTimeDTO>({
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
		new list<RunTimeDTO>({
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
	program.getStartTimes().insert(100, new StartTime()).second->set(1, 0);
	program.getStartTimes().insert(101, new StartTime()).second->set(1, 1);
	program.getStartTimes().insert(102, new StartTime()).second->set(1, 2);

	program.updateFromDTO(ProgramDTO());
	EXPECT_THAT(program.getStartTimes().size(), Eq(3));
	EXPECT_THAT(program.getStartTimes().at(100)->getStartTimeDTO(), Eq(StartTimeDTO(1, 0)));
	EXPECT_THAT(program.getStartTimes().at(101)->getStartTimeDTO(), Eq(StartTimeDTO(1, 1)));
	EXPECT_THAT(program.getStartTimes().at(102)->getStartTimeDTO(), Eq(StartTimeDTO(1, 2)));

	program.updateFromDTO(ProgramDTO().setStartTimes(
		new list<StartTimeDTO>({
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

class MockScheduler : public SpecifiedScheduler {
public:
	MOCK_CONST_METHOD1(isDayScheduled, bool(const tm&));
};

class MockSchedulerFactory : public SchedulerFactory {
public:
	virtual SpecifiedScheduler* createSpecifiedScheduler() const override {
		return new MockScheduler();
	}
};

TEST(Program, isScheduled1) {
	Program program(new MockSchedulerFactory());
	time_t t;

	program.getCurrentScheduler();
	const MockScheduler& mockScheduler = dynamic_cast<const MockScheduler&>(program.getCurrentScheduler());
	ON_CALL(mockScheduler, isDayScheduled(_)).WillByDefault(Return(false));
	EXPECT_CALL(mockScheduler, isDayScheduled(_)).Times(AnyNumber());

	program.getStartTimes().insert(0, new StartTime(4, 0, 0));
	program.getStartTimes().insert(1, new StartTime(6, 0, 0));
	program.getStartTimes().insert(2, new StartTime(6, 30, 0));
	program.getStartTimes().insert(3, new StartTime(20, 15, 0));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				t = toTime(2018, 5, 27, hour, min, sec, true);
				EXPECT_FALSE(program.isScheduled(t));
			}
		}
	}
}

TEST(Program, isScheduled2) {
	Program program(new MockSchedulerFactory());
	time_t t;

	const MockScheduler& mockScheduler = dynamic_cast<const MockScheduler&>(program.getCurrentScheduler());
	ON_CALL(mockScheduler, isDayScheduled(_)).WillByDefault(Return(true));
	EXPECT_CALL(mockScheduler, isDayScheduled(_)).Times(AnyNumber());

	program.getStartTimes().insert(0, new StartTime(4, 0, 0));
	program.getStartTimes().insert(1, new StartTime(6, 0, 0));
	program.getStartTimes().insert(2, new StartTime(6, 30, 0));
	program.getStartTimes().insert(3, new StartTime(20, 15, 0));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				t = toTime(2018, 5, 27, hour, min, sec, true);

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

