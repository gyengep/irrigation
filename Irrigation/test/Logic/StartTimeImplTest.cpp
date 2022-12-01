#include "Logic/StartTimeImpl.h"
#include "Exceptions/Exceptions.h"
#include <gmock/gmock.h>

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
