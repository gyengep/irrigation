#include "StartTimeImplTest.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeImplConstructorTest, defaultConstructor) {
	const StartTimeImpl startTime;

	EXPECT_THAT(startTime.getHours(), Eq(0));
	EXPECT_THAT(startTime.getMinutes(), Eq(0));
}

TEST(StartTimeImplConstructorTest, parametrizedConstructor) {
	const StartTimeImpl startTime(1, 30);

	EXPECT_THAT(startTime.getHours(), Eq(1));
	EXPECT_THAT(startTime.getMinutes(), Eq(30));
}

TEST(StartTimeImplConstructorTest, parametrizedConstructor_invalid) {
	EXPECT_NO_THROW(StartTimeImpl(23, 59));
	EXPECT_THROW(StartTimeImpl(24, 0), ValueOutOfBoundsException);
	EXPECT_THROW(StartTimeImpl(23, 60), ValueOutOfBoundsException);
}

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeImplFactoryTest, create) {
	EXPECT_THAT(StartTimeImplFactory().create(), Not(IsNull()));
	EXPECT_TRUE(StartTimeImplFactory().create() != StartTimeImplFactory().create());
}

///////////////////////////////////////////////////////////////////////////////

void StartTimeImplTest::SetUp() {
	startTime = std::make_shared<StartTimeImpl>();
}

void StartTimeImplTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeImplTest, less) {
	const unsigned hour = 15;
	const unsigned minute = 25;

	startTime->set(hour, minute);

	EXPECT_FALSE(startTime->less(StartTimeImpl(hour - 1, minute - 1)));
	EXPECT_FALSE(startTime->less(StartTimeImpl(hour - 1, minute)));
	EXPECT_FALSE(startTime->less(StartTimeImpl(hour - 1, minute + 1)));

	EXPECT_FALSE(startTime->less(StartTimeImpl(hour, minute - 1)));
	EXPECT_FALSE(startTime->less(StartTimeImpl(hour, minute)));
	EXPECT_TRUE(startTime->less(StartTimeImpl(hour, minute + 1)));

	EXPECT_TRUE(startTime->less(StartTimeImpl(hour + 1, minute - 1)));
	EXPECT_TRUE(startTime->less(StartTimeImpl(hour + 1, minute)));
	EXPECT_TRUE(startTime->less(StartTimeImpl(hour + 1, minute + 1)));
}

TEST_F(StartTimeImplTest, set) {
	startTime->set(21, 36);
	EXPECT_THAT(startTime->getHours(), Eq(21));
	EXPECT_THAT(startTime->getMinutes(), Eq(36));

	startTime->set(8, 12);
	EXPECT_THAT(startTime->getHours(), Eq(8));
	EXPECT_THAT(startTime->getMinutes(), Eq(12));
}

TEST_F(StartTimeImplTest, setValueMax) {
	EXPECT_NO_THROW(startTime->set(23, 59));
	EXPECT_THROW(startTime->set(24, 0), ValueOutOfBoundsException);
	EXPECT_THROW(startTime->set(23, 60), ValueOutOfBoundsException);
}
