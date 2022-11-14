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

TEST(StartTimeImplTest, less) {
	const unsigned hour = 15;
	const unsigned minute = 25;

	const StartTimeImpl startTime(hour, minute);

	EXPECT_TRUE(StartTimeImpl(hour - 1, minute - 1).less(startTime));
	EXPECT_TRUE(StartTimeImpl(hour - 1, minute).less(startTime));
	EXPECT_TRUE(StartTimeImpl(hour - 1, minute + 1).less(startTime));

	EXPECT_TRUE(StartTimeImpl(hour, minute - 1).less(startTime));
	EXPECT_FALSE(StartTimeImpl(hour, minute).less(startTime));
	EXPECT_FALSE(StartTimeImpl(hour, minute + 1).less(startTime));

	EXPECT_FALSE(StartTimeImpl(hour + 1, minute - 1).less(startTime));
	EXPECT_FALSE(StartTimeImpl(hour + 1, minute).less(startTime));
	EXPECT_FALSE(StartTimeImpl(hour + 1, minute + 1).less(startTime));
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

TEST(StartTimeImplTest, updateFromDto_empty) {
	const unsigned expectedHours = 15;
	const unsigned expectedMinutes = 16;

	StartTimeImpl actual(expectedHours, expectedMinutes);

	actual.updateFromStartTimeDto(
		StartTimeDTO()
	);

	EXPECT_THAT(actual.getHours(), Eq(expectedHours));
	EXPECT_THAT(actual.getMinutes(), Eq(expectedMinutes));
}

TEST(StartTimeImplTest, updateFromDto_partial_minutes) {
	const unsigned expectedMinutes1 = 20;
	const unsigned expectedMinutes2 = 30;

	StartTimeImpl actual(15, 16);

	actual.updateFromStartTimeDto(
		StartTimeDTO().
			setMinutes(expectedMinutes1)
	);

	EXPECT_THAT(actual.getHours(), Eq(0));
	EXPECT_THAT(actual.getMinutes(), Eq(expectedMinutes1));

	actual.updateFromStartTimeDto(
		StartTimeDTO().
			setMinutes(expectedMinutes2)
	);

	EXPECT_THAT(actual.getHours(), Eq(0));
	EXPECT_THAT(actual.getMinutes(), Eq(expectedMinutes2));
}

TEST(StartTimeImplTest, updateFromDto_partial_hours) {
	const unsigned expectedHours1 = 9;
	const unsigned expectedHours2 = 18;

	StartTimeImpl actual(15, 16);

	actual.updateFromStartTimeDto(
		StartTimeDTO().
			setHours(expectedHours1)
	);

	EXPECT_THAT(actual.getHours(), Eq(expectedHours1));
	EXPECT_THAT(actual.getMinutes(), Eq(0));

	actual.updateFromStartTimeDto(
		StartTimeDTO().
			setHours(expectedHours2)
	);

	EXPECT_THAT(actual.getHours(), Eq(expectedHours2));
	EXPECT_THAT(actual.getMinutes(), Eq(0));
}

TEST(StartTimeImplTest, updateFromDto_all) {
	const unsigned expectedHours1 = 9;
	const unsigned expectedMinutes1 = 20;
	const unsigned expectedHours2 = 18;
	const unsigned expectedMinutes2 = 30;

	StartTimeImpl actual(15, 16);

	actual.updateFromStartTimeDto(
		StartTimeDTO().
			setHours(expectedHours1).
			setMinutes(expectedMinutes1)
	);

	EXPECT_THAT(actual.getHours(), Eq(expectedHours1));
	EXPECT_THAT(actual.getMinutes(), Eq(expectedMinutes1));

	actual.updateFromStartTimeDto(
		StartTimeDTO().
			setHours(expectedHours2).
			setMinutes(expectedMinutes2)
	);

	EXPECT_THAT(actual.getHours(), Eq(expectedHours2));
	EXPECT_THAT(actual.getMinutes(), Eq(expectedMinutes2));
}
