#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/StartTime.h"
#include "Dto2Object/StartTimeSamples.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeTest, defaultConstructor) {
	StartTime startTime;
	EXPECT_THAT(startTime.getHours(), Eq(0));
	EXPECT_THAT(startTime.getMinutes(), Eq(0));
}

TEST(StartTimeTest, copyConstructor) {
	StartTime startTime(StartTime(20, 10));
	EXPECT_THAT(startTime.getHours(), Eq(20));
	EXPECT_THAT(startTime.getMinutes(), Eq(10));
}

TEST(StartTimeTest, parametrizedConstructor) {
	StartTime startTime(1, 30);
	EXPECT_THAT(startTime.getHours(), Eq(1));
	EXPECT_THAT(startTime.getMinutes(), Eq(30));
}

TEST(StartTimeTest, equalsOperator) {
	StartTime startTime;

	EXPECT_TRUE(StartTime() == startTime);

	const unsigned hour = 15;
	const unsigned minute = 25;

	startTime.set(hour, minute);
	EXPECT_FALSE(StartTime() == startTime);
	EXPECT_TRUE(StartTime(hour, minute) == startTime);
	EXPECT_FALSE(StartTime(hour, minute + 1) == startTime);
	EXPECT_FALSE(StartTime(hour, minute - 1) == startTime);
	EXPECT_FALSE(StartTime(hour + 1, minute) == startTime);
	EXPECT_FALSE(StartTime(hour - 1, minute) == startTime);
}

TEST(StartTimeTest, lessOperator) {

	const unsigned hour = 15;
	const unsigned minute = 25;

	StartTime startTime(hour, minute);

	EXPECT_TRUE(StartTime(hour - 1, minute - 1) < startTime);
	EXPECT_TRUE(StartTime(hour - 1, minute) < startTime);
	EXPECT_TRUE(StartTime(hour - 1, minute + 1) < startTime);

	EXPECT_TRUE(StartTime(hour, minute - 1) < startTime);
	EXPECT_FALSE(StartTime(hour, minute) < startTime);
	EXPECT_FALSE(StartTime(hour, minute + 1) < startTime);

	EXPECT_FALSE(StartTime(hour + 1, minute - 1) < startTime);
	EXPECT_FALSE(StartTime(hour + 1, minute) < startTime);
	EXPECT_FALSE(StartTime(hour + 1, minute + 1) < startTime);
}

TEST(StartTimeTest, equals) {
	const unsigned hour = 15;
	const unsigned minute = 25;

	StartTime startTime(hour, minute);

	EXPECT_TRUE(startTime.equals(hour, minute, 0));

	EXPECT_FALSE(startTime.equals(hour, minute, 59));
	EXPECT_FALSE(startTime.equals(hour, minute, 1));
	EXPECT_FALSE(startTime.equals(hour, minute - 1, 0));
	EXPECT_FALSE(startTime.equals(hour, minute + 1, 0));
	EXPECT_FALSE(startTime.equals(hour - 1, minute, 0));
	EXPECT_FALSE(startTime.equals(hour + 1, minute, 0));
}

TEST(StartTimeTest, set) {
	StartTime startTime;
	startTime.set(21, 36);
	EXPECT_THAT(startTime.getHours(), Eq(21));
	EXPECT_THAT(startTime.getMinutes(), Eq(36));
}

TEST(StartTimeTest, setValueMax) {
	StartTime startTime;
	EXPECT_NO_THROW(StartTime(23, 59));
	EXPECT_THROW(StartTime(24, 0), ValueOutOfBoundsException);
	EXPECT_THROW(StartTime(23, 60), ValueOutOfBoundsException);
}

///////////////////////////////////////////////////////////////////////////////

void testToStartTimeDto(const StartTimeSample& startTimeSample) {
	const shared_ptr<StartTime> startTime = startTimeSample.getObject();
	const StartTimeDTO& expectedStartTimeDto = startTimeSample.getDto();
	EXPECT_THAT(startTime->toStartTimeDto(), Eq(expectedStartTimeDto));
}

TEST(StartTimeTest, toStartTimeDto1) {
	testToStartTimeDto(StartTimeSample1());
}

TEST(StartTimeTest, toStartTimeDto2) {
	testToStartTimeDto(StartTimeSample2());
}

TEST(StartTimeTest, toStartTimeDto3) {
	testToStartTimeDto(StartTimeSample3());
}

TEST(StartTimeTest, toStartTimeDto4) {
	testToStartTimeDto(StartTimeSample4());
}

///////////////////////////////////////////////////////////////////////////////

void testUpdateFromStartTimeDto(shared_ptr<StartTime> startTime, const StartTimeSample& startTimeSample) {
	EXPECT_THAT(startTime, Not(Pointee(*startTimeSample.getObject())));
	startTime->updateFromStartTimeDto(startTimeSample.getDto());
	EXPECT_THAT(startTime, Pointee(*startTimeSample.getObject()));
}

TEST(StartTimeTest, updateFromStartTimeDto1) {
	shared_ptr<StartTime> startTime = shared_ptr<StartTime>(new StartTime());
	testUpdateFromStartTimeDto(startTime, StartTimeSample1());
}

TEST(StartTimeTest, updateFromStartTimeDto2) {
	shared_ptr<StartTime> startTime = StartTimeSample1().getObject();
	testUpdateFromStartTimeDto(startTime, StartTimeSample2());
}

TEST(StartTimeTest, updateFromStartTimeDto3) {
	shared_ptr<StartTime> startTime = StartTimeSample2().getObject();
	testUpdateFromStartTimeDto(startTime, StartTimeSample3());
}

TEST(StartTimeTest, updateFromStartTimeDto4) {
	shared_ptr<StartTime> startTime = StartTimeSample3().getObject();
	testUpdateFromStartTimeDto(startTime, StartTimeSample4());
}

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeTest, partialUpdateFromStartTimeDto_empty) {
	StartTime actualStartTime(*StartTimeSample1().getObject());

	actualStartTime.updateFromStartTimeDto(StartTimeDTO());
	EXPECT_THAT(actualStartTime, Eq(StartTime(*StartTimeSample1().getObject())));
}

TEST(StartTimeTest, partialUpdateFromStartTimeDto_minutes) {
	const unsigned minute1 = 20;
	const unsigned minute2 = 30;

	StartTime actualStartTime(*StartTimeSample2().getObject());

	actualStartTime.updateFromStartTimeDto(StartTimeDTO().setMinutes(minute1));
	EXPECT_THAT(actualStartTime, Eq(StartTime(0, minute1)));

	actualStartTime.updateFromStartTimeDto(StartTimeDTO().setMinutes(minute2));
	EXPECT_THAT(actualStartTime, Eq(StartTime(0, minute2)));
}

TEST(StartTimeTest, partialUpdateFromStartTimeDto_hours) {
	const unsigned hour1 = 10;
	const unsigned hour2 = 15;

	StartTime actualStartTime(*StartTimeSample3().getObject());

	actualStartTime.updateFromStartTimeDto(StartTimeDTO().setHours(hour1));
	EXPECT_THAT(actualStartTime, Eq(StartTime(hour1, 0)));

	actualStartTime.updateFromStartTimeDto(StartTimeDTO().setHours(hour2));
	EXPECT_THAT(actualStartTime, Eq(StartTime(hour2, 0)));
}
