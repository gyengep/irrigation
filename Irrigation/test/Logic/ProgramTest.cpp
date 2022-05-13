#include <gmock/gmock.h>
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/StartTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Utils/TimeConversion.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockScheduler.h"
#include "Dto2Object/ProgramSamples.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;
using ::testing::_;
using ::testing::Return;
using ::testing::AnyNumber;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramTest, defaultConstructor) {
	const RunTimeContainer expectedRunTimeContainer({
		RunTimeFactory().create(),
		RunTimeFactory().create(),
		RunTimeFactory().create(),
		RunTimeFactory().create(),
		RunTimeFactory().create(),
		RunTimeFactory().create()
	});

	const Program program;

	EXPECT_THAT(program.getName(), Eq(""));
	EXPECT_THAT(program.getSchedulerType(), Eq(SchedulerType::WEEKLY));
	EXPECT_THAT(program.getWeeklyScheduler(), Eq(WeeklyScheduler()));
	EXPECT_THAT(program.getRunTimes(), Eq(std::ref(expectedRunTimeContainer)));
	EXPECT_THAT(program.getStartTimes(), IsEmpty());
}

TEST(ProgramTest, parametrizedConstructor) {
	const string name("gcuuzg");
	const SchedulerType schedulerType = SchedulerType::EVERY_DAY;
	const shared_ptr<WeeklyScheduler> weeklyScheduler = WeeklySchedulerSample3().getObjectPtr();
	const shared_ptr<RunTimeContainer> runTimeContainer = RunTimeListSample2().getContainerPtr();
	const shared_ptr<StartTimeContainer> startTimeContainer = StartTimeListSample1().getContainerPtr();

	shared_ptr<Program> program = Program::Builder().
			setName(name).
			setSchedulerType(schedulerType).
			setWeeklyScheduler(weeklyScheduler).
			setRunTimeContainer(runTimeContainer).
			setStartTimeContainer(startTimeContainer).
			build();

	EXPECT_THAT(program->getName(), Eq(name));
	EXPECT_THAT(program->getSchedulerType(), Eq(schedulerType));
	EXPECT_THAT(program->getWeeklyScheduler(), Eq(*weeklyScheduler));
	EXPECT_THAT(program->getRunTimes(), Eq(std::ref(*runTimeContainer)));
	EXPECT_THAT(program->getStartTimes(), Eq(std::ref(*startTimeContainer)));

	EXPECT_THAT(&program->getWeeklyScheduler(), Eq(weeklyScheduler.get()));
	EXPECT_THAT(&program->getRunTimes(), Eq(runTimeContainer.get()));
	EXPECT_THAT(&program->getStartTimes(), Eq(startTimeContainer.get()));
}

TEST(ProgramTest, equalsOperator) {
	Program program1;
	Program program2;

	EXPECT_TRUE(program1 == program2);
	EXPECT_TRUE(program2 == program1);

	{
		const string name1("abcdefghijk");
		const string name2("98765");

		program1.setName(name1);
		EXPECT_FALSE(program1 == program2);
		EXPECT_FALSE(program2 == program1);

		program2.setName(name2);
		EXPECT_FALSE(program1 == program2);
		EXPECT_FALSE(program2 == program1);

		program1.setName(name2);
		EXPECT_TRUE(program1 == program2);
		EXPECT_TRUE(program2 == program1);
	}

	{
		const SchedulerType schedulerType1 = SchedulerType::EVERY_DAY;
		const SchedulerType schedulerType2 = SchedulerType::WEEKLY;

		program1.setSchedulerType(schedulerType1);
		EXPECT_FALSE(program1 == program2);
		EXPECT_FALSE(program2 == program1);

		program2.setSchedulerType(schedulerType2);
		EXPECT_FALSE(program1 == program2);
		EXPECT_FALSE(program2 == program1);

		program1.setSchedulerType(schedulerType2);
		EXPECT_TRUE(program1 == program2);
		EXPECT_TRUE(program2 == program1);
	}

	{
		auto program1 = Program::Builder().setWeeklyScheduler(WeeklySchedulerSample2().getObjectPtr()).build();
		auto program2 = Program::Builder().setWeeklyScheduler(WeeklySchedulerSample3().getObjectPtr()).build();
		auto program3 = Program::Builder().setWeeklyScheduler(WeeklySchedulerSample3().getObjectPtr()).build();

		EXPECT_FALSE(*program1 == *program2);
		EXPECT_FALSE(*program2 == *program1);
		EXPECT_TRUE(*program2 == *program3);
		EXPECT_TRUE(*program3 == *program2);
	}

	{
		auto program1 = Program::Builder().setRunTimeContainer(RunTimeListSample2().getContainerPtr()).build();
		auto program2 = Program::Builder().setRunTimeContainer(RunTimeListSample3().getContainerPtr()).build();
		auto program3 = Program::Builder().setRunTimeContainer(RunTimeListSample3().getContainerPtr()).build();

		EXPECT_FALSE(*program1 == *program2);
		EXPECT_FALSE(*program2 == *program1);
		EXPECT_TRUE(*program2 == *program3);
		EXPECT_TRUE(*program3 == *program2);
	}

	{
		auto program1 = Program::Builder().setStartTimeContainer(StartTimeListSample2().getContainerPtr()).build();
		auto program2 = Program::Builder().setStartTimeContainer(StartTimeListSample3().getContainerPtr()).build();
		auto program3 = Program::Builder().setStartTimeContainer(StartTimeListSample3().getContainerPtr()).build();

		EXPECT_FALSE(*program1 == *program2);
		EXPECT_FALSE(*program2 == *program1);
		EXPECT_TRUE(*program2 == *program3);
		EXPECT_TRUE(*program3 == *program2);
	}
}

TEST(ProgramTest, setName) {
	const string expected1("AbcXyz");
	const string expected2("9876543210");

	Program program;

	program.setName(expected1);
	EXPECT_THAT(program.getName(), Eq(expected1));

	program.setName(expected2);
	EXPECT_THAT(program.getName(), Eq(expected2));
}

TEST(ProgramTest, setSchedulerType) {
	const SchedulerType expected1 = SchedulerType::EVERY_DAY;
	const SchedulerType expected2 = SchedulerType::WEEKLY;

	Program program;

	program.setSchedulerType(expected1);
	EXPECT_THAT(program.getSchedulerType(), Eq(expected1));

	program.setSchedulerType(expected2);
	EXPECT_THAT(program.getSchedulerType(), Eq(expected2));
}

TEST(ProgramTest, isScheduled1) {
	MockProgram program;
	MockScheduler scheduler;

	EXPECT_CALL(program, getCurrentScheduler()).
			WillRepeatedly(ReturnRef(scheduler));
	EXPECT_CALL(scheduler, process(_)).
			Times(4).
			WillRepeatedly(Return(Scheduler::Result(false, false, 0)));

	program.setAdjustment(76);
	program.getStartTimes().insert(0, std::make_shared<StartTimeImpl>(4, 0));
	program.getStartTimes().insert(1, std::make_shared<StartTimeImpl>(6, 0));
	program.getStartTimes().insert(2, std::make_shared<StartTimeImpl>(6, 30));
	program.getStartTimes().insert(3, std::make_shared<StartTimeImpl>(20, 15));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {

				bool expectedResult = false;
				expectedResult |= (hour == 4 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 30 && sec == 0);
				expectedResult |= (hour == 20 && min == 15 && sec == 0);

				const time_t rawTime = fromLocalTime(2018, 5, 27, hour, min, sec);
				ASSERT_THAT(program.isScheduled(rawTime), Pointee(ScheduledResult(expectedResult, 0)));
			}
		}
	}
}

TEST(ProgramTest, isScheduled2) {
	MockProgram program;
	MockScheduler scheduler;

	EXPECT_CALL(program, getCurrentScheduler()).
			WillRepeatedly(ReturnRef(scheduler));
	EXPECT_CALL(scheduler, process(_)).
			Times(4).
			WillRepeatedly(Return(Scheduler::Result(true, false, 0)));

	program.setAdjustment(76);
	program.getStartTimes().insert(0, std::make_shared<StartTimeImpl>(4, 0));
	program.getStartTimes().insert(1, std::make_shared<StartTimeImpl>(6, 0));
	program.getStartTimes().insert(2, std::make_shared<StartTimeImpl>(6, 30));
	program.getStartTimes().insert(3, std::make_shared<StartTimeImpl>(20, 15));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				bool expectedResult = false;
				expectedResult |= (hour == 4 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 30 && sec == 0);
				expectedResult |= (hour == 20 && min == 15 && sec == 0);

				const time_t rawTime = fromLocalTime(2018, 5, 27, hour, min, sec);
				ASSERT_THAT(program.isScheduled(rawTime), Pointee(ScheduledResult(expectedResult, expectedResult ? 76 : 0)));
			}
		}
	}
}

TEST(ProgramTest, isScheduled3) {
	MockProgram program;
	MockScheduler scheduler;

	EXPECT_CALL(program, getCurrentScheduler()).
			WillRepeatedly(ReturnRef(scheduler));
	EXPECT_CALL(scheduler, process(_)).
			Times(4).
			WillRepeatedly(Return(Scheduler::Result(true, true, 25)));

	program.setAdjustment(76);
	program.getStartTimes().insert(0, std::make_shared<StartTimeImpl>(4, 0));
	program.getStartTimes().insert(1, std::make_shared<StartTimeImpl>(6, 0));
	program.getStartTimes().insert(2, std::make_shared<StartTimeImpl>(6, 30));
	program.getStartTimes().insert(3, std::make_shared<StartTimeImpl>(20, 15));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				bool expectedResult = false;
				expectedResult |= (hour == 4 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 0 && sec == 0);
				expectedResult |= (hour == 6 && min == 30 && sec == 0);
				expectedResult |= (hour == 20 && min == 15 && sec == 0);

				const time_t rawTime = fromLocalTime(2018, 5, 27, hour, min, sec);
				ASSERT_THAT(program.isScheduled(rawTime), Pointee(ScheduledResult(expectedResult, expectedResult ? 0.25f * 0.76f * 100.0f : 0)));
			}
		}
	}
}

TEST(ProgramTest, isScheduled_disabled) {
	MockProgram program;
	MockScheduler scheduler;

	program.setEnabled(false);
	program.getStartTimes().insert(0, std::make_shared<StartTimeImpl>(4, 0));
	program.getStartTimes().insert(1, std::make_shared<StartTimeImpl>(6, 0));
	program.getStartTimes().insert(2, std::make_shared<StartTimeImpl>(6, 30));
	program.getStartTimes().insert(3, std::make_shared<StartTimeImpl>(20, 15));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				const time_t rawTime = fromLocalTime(2018, 5, 27, hour, min, sec);
				ASSERT_THAT(program.isScheduled(rawTime), Pointee(ScheduledResult(false, 0)));
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void testToProgramDto(const ProgramSample& programSample) {
	EXPECT_THAT(programSample.getObjectPtr()->toProgramDto(), Eq(programSample.getDto()));
}

TEST(ProgramTest, toProgramDto1) {
	testToProgramDto(ProgramSample1());
}

TEST(ProgramTest, toProgramDto2) {
	testToProgramDto(ProgramSample2());
}

TEST(ProgramTest, toProgramDto3) {
	testToProgramDto(ProgramSample3());
}

TEST(ProgramTest, toProgramDto4) {
	testToProgramDto(ProgramSample4());
}

///////////////////////////////////////////////////////////////////////////////

void testUpdateFromProgramDto(const shared_ptr<Program>& program, const ProgramSample& programSample) {
	EXPECT_THAT(program, Pointee(Not(Eq(std::ref(*programSample.getObjectPtr())))));
	program->updateFromProgramDto(programSample.getDto());
	EXPECT_THAT(program, Pointee(Eq(std::ref(*programSample.getObjectPtr()))));
}

TEST(ProgramTest, updateFromProgramDto1) {
	shared_ptr<Program> program = Program::Builder().build();
	testUpdateFromProgramDto(program, ProgramSample1());
}

TEST(ProgramTest, updateFromProgramDto2) {
	shared_ptr<Program> program = ProgramSample1().getObjectPtr();
	testUpdateFromProgramDto(program, ProgramSample2());
}

TEST(ProgramTest, updateFromProgramDto3) {
	shared_ptr<Program> program = ProgramSample2().getObjectPtr();
	testUpdateFromProgramDto(program, ProgramSample3());
}

TEST(ProgramTest, updateFromProgramDto4) {
	shared_ptr<Program> program = ProgramSample3().getObjectPtr();
	testUpdateFromProgramDto(program, ProgramSample4());
}

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramTest, partialUpdateFromProgramDto_empty) {
	const ProgramPtr expected = ProgramSample1().getObjectPtr();

	ProgramPtr actual = ProgramSample1().getObjectPtr();
	actual->updateFromProgramDto(ProgramDTO());
	EXPECT_THAT(actual, Pointee(Eq(std::ref(*expected))));
}

TEST(ProgramTest, partialUpdateFromProgramDto_name) {
	const ProgramSample2 programSample;
	const ProgramPtr expected1 = programSample.createBuilder()->setName("abcdefg").build();
	const ProgramPtr expected2 = programSample.createBuilder()->setName("123456789").build();

	ProgramPtr actual = programSample.getObjectPtr();

	actual->updateFromProgramDto(ProgramDTO().setName("abcdefg"));
	EXPECT_THAT(actual, Pointee(Eq(std::ref(*expected1))));

	actual->updateFromProgramDto(ProgramDTO().setName("123456789"));
	EXPECT_THAT(actual, Pointee(Eq(std::ref(*expected2))));
}

TEST(ProgramTest, partialUpdateFromProgramDto_schedulerType) {
	const ProgramSample3 programSample;
	const ProgramPtr expected1 = programSample.createBuilder()->setSchedulerType(SchedulerType::EVERY_DAY).build();
	const ProgramPtr expected2 = programSample.createBuilder()->setSchedulerType(SchedulerType::WEEKLY).build();

	ProgramPtr actual = programSample.getObjectPtr();

	actual->updateFromProgramDto(ProgramDTO().setSchedulerType(to_string(SchedulerType::EVERY_DAY)));
	EXPECT_THAT(actual, Pointee(Eq(std::ref(*expected1))));

	actual->updateFromProgramDto(ProgramDTO().setSchedulerType(to_string(SchedulerType::WEEKLY)));
	EXPECT_THAT(actual, Pointee(Eq(std::ref(*expected2))));
}

TEST(ProgramTest, partialUpdateFromProgramDto_weeklyScheduler) {
	const ProgramSample1 programSample;
	const ProgramPtr expected1 = programSample.createBuilder()->setWeeklyScheduler(WeeklySchedulerSample1().getObjectPtr()).build();
	const ProgramPtr expected2 = programSample.createBuilder()->setWeeklyScheduler(WeeklySchedulerSample2().getObjectPtr()).build();

	ProgramPtr actual = programSample.getObjectPtr();

	actual->updateFromProgramDto(ProgramDTO().setWeeklyScheduler(WeeklySchedulerDTO(WeeklySchedulerSample1().getDto())));
	EXPECT_THAT(actual, Pointee(Eq(std::ref(*expected1))));

	actual->updateFromProgramDto(ProgramDTO().setWeeklyScheduler(WeeklySchedulerDTO(WeeklySchedulerSample2().getDto())));
	EXPECT_THAT(actual, Pointee(Eq(std::ref(*expected2))));
}

TEST(ProgramTest, partialUpdateFromProgramDto_runTimeContainer) {
	const ProgramSample2 programSample;
	const ProgramPtr expected1 = programSample.createBuilder()->setRunTimeContainer(RunTimeListSample1().getContainerPtr()).build();
	const ProgramPtr expected2 = programSample.createBuilder()->setRunTimeContainer(RunTimeListSample2().getContainerPtr()).build();

	ProgramPtr actual = programSample.getObjectPtr();

	actual->updateFromProgramDto(ProgramDTO().setRunTimes(list<RunTimeDTO>(RunTimeListSample1().getDtoList())));
	EXPECT_THAT(actual, Pointee(Eq(std::ref(*expected1))));

	actual->updateFromProgramDto(ProgramDTO().setRunTimes(list<RunTimeDTO>(RunTimeListSample2().getDtoList())));
	EXPECT_THAT(actual, Pointee(Eq(std::ref(*expected2))));
}

TEST(ProgramTest, partialUpdateFromProgramDto_startTimeContainer) {
	const ProgramSample3 programSample;
	const ProgramPtr expected1 = programSample.createBuilder()->setStartTimeContainer(StartTimeListSample1().getContainerPtr()).build();
	const ProgramPtr expected2 = programSample.createBuilder()->setStartTimeContainer(StartTimeListSample2().getContainerPtr()).build();

	ProgramPtr actual = programSample.getObjectPtr();

	actual->updateFromProgramDto(ProgramDTO().setStartTimes(list<StartTimeDTO>(StartTimeListSample1().getDtoList())));
	EXPECT_THAT(actual, Pointee(Eq(std::ref(*expected1))));

	actual->updateFromProgramDto(ProgramDTO().setStartTimes(list<StartTimeDTO>(StartTimeListSample2().getDtoList())));
	EXPECT_THAT(actual, Pointee(Eq(std::ref(*expected2))));
}
