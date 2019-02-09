#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/RunTime.h"
#include "Dto2Object/RunTimeSamples.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeTest, defaultConstructor) {
	RunTime runTime;
	EXPECT_THAT(runTime.getSeconds(), Eq(0));
}

TEST(RunTimeTest, parametrizedConstructor) {
	RunTime runTime(10);
	EXPECT_THAT(runTime.getSeconds(), Eq(10));
}

TEST(RunTimeTest, copyConstructor) {
	RunTime runTime(RunTime(20));
	EXPECT_THAT(runTime.getSeconds(), Eq(20));
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
	EXPECT_THAT(runTimeSample.getObject()->toRunTimeDto(), Eq(runTimeSample.getDto()));
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
	EXPECT_THAT(runTime, Not(Pointee(*runTimeSample.getObject())));
	runTime->updateFromRunTimeDto(runTimeSample.getDto());
	EXPECT_THAT(runTime, Pointee(*runTimeSample.getObject()));
}

TEST(RunTimeTest, updateFromRunTimeDto1) {
	shared_ptr<RunTime> runTime = shared_ptr<RunTime>(new RunTime());
	testUpdateFromRunTimeDto(runTime, RunTimeSample1());
}

TEST(RunTimeTest, updateFromRunTimeDto2) {
	shared_ptr<RunTime> runTime = RunTimeSample1().getObject();
	testUpdateFromRunTimeDto(runTime, RunTimeSample2());
}

TEST(RunTimeTest, updateFromRunTimeDto3) {
	shared_ptr<RunTime> runTime = RunTimeSample2().getObject();
	testUpdateFromRunTimeDto(runTime, RunTimeSample3());
}

TEST(RunTimeTest, updateFromRunTimeDto4) {
	shared_ptr<RunTime> runTime = RunTimeSample3().getObject();
	testUpdateFromRunTimeDto(runTime, RunTimeSample4());
}

TEST(RunTimeTest, updateFromRunTimeDto5) {
	shared_ptr<RunTime> runTime = RunTimeSample4().getObject();
	testUpdateFromRunTimeDto(runTime, RunTimeSample5());
}

TEST(RunTimeTest, updateFromRunTimeDto6) {
	shared_ptr<RunTime> runTime = RunTimeSample5().getObject();
	testUpdateFromRunTimeDto(runTime, RunTimeSample6());
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeTest, partialUpdateFromRunTimeDto_empty) {
	RunTime actualRunTime(*RunTimeSample1().getObject());

	actualRunTime.updateFromRunTimeDto(RunTimeDTO());
	EXPECT_THAT(actualRunTime, RunTime(*RunTimeSample1().getObject()));
}

TEST(RunTimeTest, partialUpdateFromRunTimeDto_seconds) {
	const unsigned second1 = 20;
	const unsigned second2 = 30;

	RunTime actualRunTime(*RunTimeSample2().getObject());

	actualRunTime.updateFromRunTimeDto(RunTimeDTO().setSeconds(second1));
	EXPECT_THAT(actualRunTime, Eq(RunTime(second1)));

	actualRunTime.updateFromRunTimeDto(RunTimeDTO().setSeconds(second2));
	EXPECT_THAT(actualRunTime, Eq(RunTime(second2)));
}

TEST(RunTimeTest, partialUpdateFromRunTimeDto_minutes) {
	const unsigned minute1 = 55;
	const unsigned minute2 = 40;

	RunTime actualRunTime(*RunTimeSample3().getObject());

	actualRunTime.updateFromRunTimeDto(RunTimeDTO().setMinutes(minute1));
	EXPECT_THAT(actualRunTime, Eq(RunTime(minute1 * 60)));

	actualRunTime.updateFromRunTimeDto(RunTimeDTO().setMinutes(minute2));
	EXPECT_THAT(actualRunTime, Eq(RunTime(minute2 * 60)));
}
