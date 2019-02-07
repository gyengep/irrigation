#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/RunTime.h"
#include "RunTimeSamples.h"

using namespace std;
using namespace testing;
using namespace LogicTest;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeTest, defaultConstructor) {
	RunTime runTime;
	EXPECT_THAT(runTime.getSeconds(), Eq(0));
}

TEST(RunTimeTest, copyConstructor) {
	RunTime runTime(RunTime(20));
	EXPECT_THAT(runTime.getSeconds(), Eq(20));
}

TEST(RunTimeTest, parametrizedConstructor) {
	RunTime runTime(10);
	EXPECT_THAT(runTime.getSeconds(), Eq(10));
}

TEST(RunTimeTest, equalsOperator) {
	EXPECT_TRUE(RunTime(10) == RunTime(10));
	EXPECT_FALSE(RunTime(10) == RunTime(11));
	EXPECT_FALSE(RunTime(11) == RunTime(10));
}

TEST(RunTimeTest, setValue) {
	RunTime runTime;
	runTime.setSeconds(25);
	EXPECT_THAT(runTime.getSeconds(), Eq(25));
}

TEST(RunTimeTest, setValueMax) {
	RunTime runTime;
	EXPECT_NO_THROW(runTime.setSeconds(24 * 60 * 60));
	EXPECT_THROW(runTime.setSeconds(24 * 60 * 60 + 1), ValueOutOfBoundsException);
}

///////////////////////////////////////////////////////////////////////////////

void testToRunTimeDto(const RunTimeSample& runTimeSample) {
	const shared_ptr<RunTime> runTime = runTimeSample.getRunTime();
	const RunTimeDTO& expectedRunTimeDto = runTimeSample.getRunTimeDto();
	EXPECT_THAT(runTime->toRunTimeDto(), Eq(expectedRunTimeDto));
}

TEST(RunTimeTest, toRunTimeDto1) {
	testToRunTimeDto(RunTimeSample1());
}

TEST(RunTimeTest, toRunTimeDto2) {
	testToRunTimeDto(RunTimeSample2());
}

TEST(RunTimeTest, toRunTimeDto3) {
	testToRunTimeDto(RunTimeSample3());
}

TEST(RunTimeTest, toRunTimeDto4) {
	testToRunTimeDto(RunTimeSample4());
}

TEST(RunTimeTest, toRunTimeDto5) {
	testToRunTimeDto(RunTimeSample5());
}

TEST(RunTimeTest, toRunTimeDto6) {
	testToRunTimeDto(RunTimeSample6());
}

///////////////////////////////////////////////////////////////////////////////

void testUpdateFromRunTimeDto(shared_ptr<RunTime> runTime, const RunTimeSample& runTimeSample) {
	EXPECT_THAT(runTime, Not(Pointee(*runTimeSample.getRunTime())));
	runTime->updateFromRunTimeDto(runTimeSample.getRunTimeDto());
	EXPECT_THAT(runTime, Pointee(*runTimeSample.getRunTime()));
}

TEST(RunTimeTest, updateFromRunTimeDto1) {
	shared_ptr<RunTime> runTime = shared_ptr<RunTime>(new RunTime());
	testUpdateFromRunTimeDto(runTime, RunTimeSample1());
}

TEST(RunTimeTest, updateFromRunTimeDto2) {
	shared_ptr<RunTime> runTime = RunTimeSample1().getRunTime();
	testUpdateFromRunTimeDto(runTime, RunTimeSample2());
}

TEST(RunTimeTest, updateFromRunTimeDto3) {
	shared_ptr<RunTime> runTime = RunTimeSample2().getRunTime();
	testUpdateFromRunTimeDto(runTime, RunTimeSample3());
}

TEST(RunTimeTest, updateFromRunTimeDto4) {
	shared_ptr<RunTime> runTime = RunTimeSample3().getRunTime();
	testUpdateFromRunTimeDto(runTime, RunTimeSample4());
}

TEST(RunTimeTest, updateFromRunTimeDto5) {
	shared_ptr<RunTime> runTime = RunTimeSample4().getRunTime();
	testUpdateFromRunTimeDto(runTime, RunTimeSample5());
}

TEST(RunTimeTest, updateFromRunTimeDto6) {
	shared_ptr<RunTime> runTime = RunTimeSample5().getRunTime();
	testUpdateFromRunTimeDto(runTime, RunTimeSample6());
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeTest, partialUpdateFromRunTimeDto_empty) {
	RunTimeSample1 runTimeSample;
	runTimeSample.getRunTime()->updateFromRunTimeDto(RunTimeDTO());
	EXPECT_THAT(runTimeSample.getRunTime(), Pointee(*RunTimeSample1().getRunTime()));
}

TEST(RunTimeTest, partialUpdateFromRunTimeDto_seconds) {
	RunTimeSample2 runTimeSample;
	runTimeSample.getRunTime()->updateFromRunTimeDto(RunTimeDTO().setSeconds(130));
	EXPECT_THAT(runTimeSample.getRunTime(), Pointee(RunTime(130)));
}

TEST(RunTimeTest, partialUpdateFromRunTimeDto_minutes) {
	RunTimeSample3 runTimeSample;
	runTimeSample.getRunTime()->updateFromRunTimeDto(RunTimeDTO().setMinutes(3));
	EXPECT_THAT(runTimeSample.getRunTime(), Pointee(RunTime(180)));
}
