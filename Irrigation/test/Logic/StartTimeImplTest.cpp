#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/StartTimeImpl.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeImplTest, defaultConstructor) {
	const StartTimeImpl startTime;

	EXPECT_THAT(startTime.getHours(), Eq(0));
	EXPECT_THAT(startTime.getMinutes(), Eq(0));
}

TEST(StartTimeImplTest, parametrizedConstructor) {
	const StartTimeImpl startTime(1, 30);

	EXPECT_THAT(startTime.getHours(), Eq(1));
	EXPECT_THAT(startTime.getMinutes(), Eq(30));
}

TEST(StartTimeImplTest, parametrizedConstructor_invalid) {
	EXPECT_NO_THROW(StartTimeImpl(23, 59));
	EXPECT_THROW(StartTimeImpl(24, 0), ValueOutOfBoundsException);
	EXPECT_THROW(StartTimeImpl(23, 60), ValueOutOfBoundsException);
}

TEST(StartTimeImplTest, equalsOperator) {
	const unsigned hour = 15;
	const unsigned minute = 25;

	const StartTimeImpl startTime(15, 25);

	EXPECT_FALSE(StartTimeImpl() == startTime);
	EXPECT_TRUE(StartTimeImpl(hour, minute) == startTime);
	EXPECT_FALSE(StartTimeImpl(hour, minute + 1) == startTime);
	EXPECT_FALSE(StartTimeImpl(hour, minute - 1) == startTime);
	EXPECT_FALSE(StartTimeImpl(hour + 1, minute) == startTime);
	EXPECT_FALSE(StartTimeImpl(hour - 1, minute) == startTime);
}

TEST(StartTimeImplTest, lessOperator) {
	const unsigned hour = 15;
	const unsigned minute = 25;

	const StartTimeImpl startTime(hour, minute);

	EXPECT_TRUE(StartTimeImpl(hour - 1, minute - 1) < startTime);
	EXPECT_TRUE(StartTimeImpl(hour - 1, minute) < startTime);
	EXPECT_TRUE(StartTimeImpl(hour - 1, minute + 1) < startTime);

	EXPECT_TRUE(StartTimeImpl(hour, minute - 1) < startTime);
	EXPECT_FALSE(StartTimeImpl(hour, minute) < startTime);
	EXPECT_FALSE(StartTimeImpl(hour, minute + 1) < startTime);

	EXPECT_FALSE(StartTimeImpl(hour + 1, minute - 1) < startTime);
	EXPECT_FALSE(StartTimeImpl(hour + 1, minute) < startTime);
	EXPECT_FALSE(StartTimeImpl(hour + 1, minute + 1) < startTime);
}

TEST(StartTimeImplTest, equals) {
	const unsigned hour = 15;
	const unsigned minute = 25;

	const StartTimeImpl startTime(hour, minute);

	EXPECT_TRUE(startTime.equals(hour, minute, 0));

	EXPECT_FALSE(startTime.equals(hour, minute, 59));
	EXPECT_FALSE(startTime.equals(hour, minute, 1));
	EXPECT_FALSE(startTime.equals(hour, minute - 1, 0));
	EXPECT_FALSE(startTime.equals(hour, minute + 1, 0));
	EXPECT_FALSE(startTime.equals(hour - 1, minute, 0));
	EXPECT_FALSE(startTime.equals(hour + 1, minute, 0));
}

TEST(StartTimeImplTest, set) {
	StartTimeImpl startTime;

	startTime.set(21, 36);
	EXPECT_THAT(startTime.getHours(), Eq(21));
	EXPECT_THAT(startTime.getMinutes(), Eq(36));

	startTime.set(8, 12);
	EXPECT_THAT(startTime.getHours(), Eq(8));
	EXPECT_THAT(startTime.getMinutes(), Eq(12));
}

TEST(StartTimeImplTest, setValueMax) {
	StartTimeImpl startTime;

	EXPECT_NO_THROW(startTime.set(23, 59));
	EXPECT_THROW(startTime.set(24, 0), ValueOutOfBoundsException);
	EXPECT_THROW(startTime.set(23, 60), ValueOutOfBoundsException);
}

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeImplTest, toStartTimeDto) {
	EXPECT_THAT(StartTimeImpl(0, 0).toStartTimeDto(), Eq(StartTimeDTO(0, 0)));
	EXPECT_THAT(StartTimeImpl(10, 0).toStartTimeDto(), Eq(StartTimeDTO(10, 0)));
	EXPECT_THAT(StartTimeImpl(0, 20).toStartTimeDto(), Eq(StartTimeDTO(0, 20)));
	EXPECT_THAT(StartTimeImpl(10, 20).toStartTimeDto(), Eq(StartTimeDTO(10, 20)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeImplTest, partialUpdateFromStartTimeDto_empty) {
	const StartTimeImpl expected(11, 12);

	StartTimeImpl actual(11, 12);
	actual.updateFromStartTimeDto(StartTimeDTO());

	EXPECT_THAT(actual, Eq(std::ref(expected)));
}

TEST(StartTimeImplTest, partialUpdateFromStartTimeDto_minutes) {
	const StartTimeImpl expected1(0, 20);
	const StartTimeImpl expected2(0, 30);

	StartTimeImpl actual(15, 16);

	actual.updateFromStartTimeDto(StartTimeDTO().setMinutes(20));
	EXPECT_THAT(actual, Eq(std::ref(expected1)));

	actual.updateFromStartTimeDto(StartTimeDTO().setMinutes(30));
	EXPECT_THAT(actual, Eq(std::ref(expected2)));
}

TEST(StartTimeImplTest, partialUpdateFromStartTimeDto_hours) {
	const StartTimeImpl expected1(9, 0);
	const StartTimeImpl expected2(20, 0);

	StartTimeImpl actual(15, 16);

	actual.updateFromStartTimeDto(StartTimeDTO().setHours(9));
	EXPECT_THAT(actual, Eq(std::ref(expected1)));

	actual.updateFromStartTimeDto(StartTimeDTO().setHours(20));
	EXPECT_THAT(actual, Eq(std::ref(expected2)));
}

TEST(StartTimeImplTest, partialUpdateFromStartTimeDto_all) {
	const StartTimeImpl expected1(9, 31);
	const StartTimeImpl expected2(20, 41);

	StartTimeImpl actual(15, 16);

	actual.updateFromStartTimeDto(StartTimeDTO().setHours(9).setMinutes(31));
	EXPECT_THAT(actual, Eq(std::ref(expected1)));

	actual.updateFromStartTimeDto(StartTimeDTO().setHours(20).setMinutes(41));
	EXPECT_THAT(actual, Eq(std::ref(expected2)));
}
