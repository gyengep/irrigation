#include <gmock/gmock.h>
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/StartTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Schedulers/PeriodicScheduler.h"
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
	auto program = Program::Builder().build();

	EXPECT_TRUE(program->getName().empty());
	EXPECT_THAT(program->getSchedulerType(), Eq(SchedulerType::WEEKLY));
	EXPECT_NO_THROW(program->getPeriodicScheduler());
	EXPECT_NO_THROW(program->getWeeklyScheduler());
}

TEST(ProgramTest, setName) {
	const string name("AbcXyz");

	auto program = Program::Builder().build();
	program->setName(name);
	EXPECT_THAT(program->getName(), Eq(name));
}

TEST(ProgramTest, setSchedulerType) {
	auto program = Program::Builder().build();
	program->setSchedulerType(SchedulerType::PERIODIC);
	EXPECT_THAT(program->getSchedulerType(), Eq(SchedulerType::PERIODIC));
}

TEST(ProgramTest, isScheduled1) {
	MockProgram program;
	MockScheduler scheduler;

	EXPECT_CALL(program, isScheduled(_)).
			Times(AnyNumber()).
			WillRepeatedly(Invoke(&program, &MockProgram::programIsScheduled));
	EXPECT_CALL(program, getCurrentScheduler()).
			Times(4).
			WillRepeatedly(ReturnRef(scheduler));
	EXPECT_CALL(scheduler, isDayScheduled(_)).
			Times(4).
			WillRepeatedly(Return(false));

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
	MockProgram program;
	MockScheduler scheduler;

	EXPECT_CALL(program, isScheduled(_)).
			Times(AnyNumber()).
			WillRepeatedly(Invoke(&program, &MockProgram::programIsScheduled));
	EXPECT_CALL(program, getCurrentScheduler()).
			Times(4).
			WillRepeatedly(ReturnRef(scheduler));
	EXPECT_CALL(scheduler, isDayScheduled(_)).
			Times(4).
			WillRepeatedly(Return(true));

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

void testToProgramDto(const ProgramSample& programSample) {
	EXPECT_THAT(programSample.getObject()->toProgramDto(), Eq(programSample.getDto()));
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

void testUpdateFromProgramDto(shared_ptr<Program> program, const ProgramSample& programSample) {
	EXPECT_THAT(program, Not(Pointee(*programSample.getObject())));
	program->updateFromProgramDto(programSample.getDto());
	EXPECT_THAT(program, Pointee(*programSample.getObject()));
}

TEST(ProgramTest, updateFromProgramDto1) {
	shared_ptr<Program> program = Program::Builder().build();
	testUpdateFromProgramDto(program, ProgramSample1());
}

TEST(ProgramTest, updateFromProgramDto2) {
	shared_ptr<Program> program = ProgramSample1().getObject();
	testUpdateFromProgramDto(program, ProgramSample2());
}

TEST(ProgramTest, updateFromProgramDto3) {
	shared_ptr<Program> program = ProgramSample2().getObject();
	testUpdateFromProgramDto(program, ProgramSample3());
}

TEST(ProgramTest, updateFromProgramDto4) {
	shared_ptr<Program> program = ProgramSample3().getObject();
	testUpdateFromProgramDto(program, ProgramSample4());
}

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramTest, partialUpdateFromProgramDto_empty) {
	Program actual(*ProgramSample1().getObject());
	Program expected(*ProgramSample1().getObject());

	actual.updateFromProgramDto(ProgramDTO());
	EXPECT_THAT(actual, Eq(expected));
}

unique_ptr<Program::Builder> createBuilder(const Program& program) {
	unique_ptr<Program::Builder> builder(new Program::Builder());
	builder->setName(program.getName());
	builder->setSchedulerType(program.getSchedulerType());
	builder->setPeriodicScheduler(shared_ptr<PeriodicScheduler>(new PeriodicScheduler(program.getPeriodicScheduler())));
	builder->setWeeklyScheduler(shared_ptr<WeeklyScheduler>(new WeeklyScheduler(program.getWeeklyScheduler())));
	builder->setRunTimeContainer(shared_ptr<RunTimeContainer>(new RunTimeContainer(program.getRunTimes())));
	builder->setStartTimeContainer(shared_ptr<StartTimeContainer>(new StartTimeContainer(program.getStartTimes())));
	return builder;
}

TEST(ProgramTest, partialUpdateFromProgramDto_name) {
	Program actual(*ProgramSample2().getObject());

	unique_ptr<Program::Builder> builder = createBuilder(actual);

	const Program expected1(*builder->setName("abcdefg").build());
	const Program expected2(*builder->setName("123456789").build());

	actual.updateFromProgramDto(ProgramDTO().setName("abcdefg"));
	EXPECT_THAT(actual, Eq(expected1));

	actual.updateFromProgramDto(ProgramDTO().setName("123456789"));
	EXPECT_THAT(actual, Eq(expected2));
}

TEST(ProgramTest, partialUpdateFromProgramDto_schedulerType) {
	Program actual(*ProgramSample3().getObject());

	unique_ptr<Program::Builder> builder = createBuilder(actual);

	const Program expected1(*builder->setSchedulerType(SchedulerType::PERIODIC).build());
	const Program expected2(*builder->setSchedulerType(SchedulerType::WEEKLY).build());

	actual.updateFromProgramDto(ProgramDTO().setSchedulerType(to_string(SchedulerType::PERIODIC)));
	EXPECT_THAT(actual, Eq(expected1));

	actual.updateFromProgramDto(ProgramDTO().setSchedulerType(to_string(SchedulerType::WEEKLY)));
	EXPECT_THAT(actual, Eq(expected2));
}

TEST(ProgramTest, partialUpdateFromProgramDto_periodicScheduler) {
	Program actual(*ProgramSample4().getObject());

	unique_ptr<Program::Builder> builder = createBuilder(actual);

	const Program expected1(*builder->setPeriodicScheduler(PeriodicSchedulerSample1().getObject()).build());
	const Program expected2(*builder->setPeriodicScheduler(PeriodicSchedulerSample2().getObject()).build());

	actual.updateFromProgramDto(ProgramDTO().setPeriodicScheduler(PeriodicSchedulerDTO(PeriodicSchedulerSample1().getDto())));
	EXPECT_THAT(actual, Eq(expected1));

	actual.updateFromProgramDto(ProgramDTO().setPeriodicScheduler(PeriodicSchedulerDTO(PeriodicSchedulerSample2().getDto())));
	EXPECT_THAT(actual, Eq(expected2));
}

TEST(ProgramTest, partialUpdateFromProgramDto_weeklyScheduler) {
	Program actual(*ProgramSample1().getObject());

	unique_ptr<Program::Builder> builder = createBuilder(actual);

	const Program expected1(*builder->setWeeklyScheduler(WeeklySchedulerSample1().getObject()).build());
	const Program expected2(*builder->setWeeklyScheduler(WeeklySchedulerSample2().getObject()).build());

	actual.updateFromProgramDto(ProgramDTO().setWeeklyScheduler(WeeklySchedulerDTO(WeeklySchedulerSample1().getDto())));
	EXPECT_THAT(actual, Eq(expected1));

	actual.updateFromProgramDto(ProgramDTO().setWeeklyScheduler(WeeklySchedulerDTO(WeeklySchedulerSample2().getDto())));
	EXPECT_THAT(actual, Eq(expected2));
}

TEST(ProgramTest, partialUpdateFromProgramDto_runTimeContainer) {
	Program actual(*ProgramSample2().getObject());

	unique_ptr<Program::Builder> builder = createBuilder(actual);

	const Program expected1(*builder->setRunTimeContainer(RunTimeListSample1().getContainer()).build());
	const Program expected2(*builder->setRunTimeContainer(RunTimeListSample2().getContainer()).build());

	actual.updateFromProgramDto(ProgramDTO().setRunTimes(list<RunTimeDTO>(RunTimeListSample1().getDtoList())));
	EXPECT_THAT(actual, Eq(expected1));

	actual.updateFromProgramDto(ProgramDTO().setRunTimes(list<RunTimeDTO>(RunTimeListSample2().getDtoList())));
	EXPECT_THAT(actual, Eq(expected2));
}

TEST(ProgramTest, partialUpdateFromProgramDto_startTimeContainer) {
	Program actual(*ProgramSample3().getObject());

	unique_ptr<Program::Builder> builder = createBuilder(actual);

	const Program expected1(*builder->setStartTimeContainer(StartTimeListSample1().getContainer()).build());
	const Program expected2(*builder->setStartTimeContainer(StartTimeListSample2().getContainer()).build());

	actual.updateFromProgramDto(ProgramDTO().setStartTimes(list<StartTimeDTO>(StartTimeListSample1().getDtoList())));
	EXPECT_THAT(actual, Eq(expected1));

	actual.updateFromProgramDto(ProgramDTO().setStartTimes(list<StartTimeDTO>(StartTimeListSample2().getDtoList())));
	EXPECT_THAT(actual, Eq(expected2));
}
