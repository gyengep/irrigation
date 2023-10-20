#include "RunTimeImplTest.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeImplConstructorTest, defaultConstructor) {
	RunTimeImpl runTime;
	EXPECT_THAT(runTime.get(), Eq(std::chrono::seconds(0)));
}

TEST(RunTimeImplConstructorTest, parametrizedConstructor) {
	RunTimeImpl runTime(std::chrono::seconds(10));
	EXPECT_THAT(runTime.get(), Eq(std::chrono::seconds(10)));

	EXPECT_NO_THROW(RunTimeImpl(std::chrono::hours(24)));
	EXPECT_THROW(RunTimeImpl(std::chrono::hours(24) + std::chrono::seconds(1)), ValueOutOfBoundsException);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeImplFactoryTest, create) {
	EXPECT_THAT(RunTimeImplFactory().create(), Not(IsNull()));
}

///////////////////////////////////////////////////////////////////////////////

void RunTimeImplTest::SetUp() {
	runTime = std::make_shared<RunTimeImpl>();
}

void RunTimeImplTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeImplTest, setValue) {
	runTime->set(std::chrono::seconds(25));
	EXPECT_THAT(runTime->get(), Eq(std::chrono::seconds(25)));

	runTime->set(std::chrono::seconds(38));
	EXPECT_THAT(runTime->get(), Eq(std::chrono::seconds(38)));
}

TEST_F(RunTimeImplTest, setValueMax) {
	EXPECT_NO_THROW(RunTimeImpl().set(std::chrono::hours(24)));
	EXPECT_THROW(RunTimeImpl().set(std::chrono::hours(24) + std::chrono::seconds(1)), ValueOutOfBoundsException);
}

TEST_F(RunTimeImplTest, toDuration) {
	EXPECT_THAT(RunTimeImpl(std::chrono::seconds(0)).toDuration(), Eq(std::chrono::seconds(0)));
	EXPECT_THAT(RunTimeImpl(std::chrono::seconds(12)).toDuration(), Eq(std::chrono::seconds(12)));
	EXPECT_THAT(RunTimeImpl(std::chrono::seconds(85)).toDuration(), Eq(std::chrono::seconds(85)));
}

TEST_F(RunTimeImplTest, toDurationStatic) {
	EXPECT_THAT(RunTime::toDuration(RunTimeDto(0, 0)), Eq(std::chrono::minutes(0) + std::chrono::seconds(0)));
	EXPECT_THAT(RunTime::toDuration(RunTimeDto(0, 12)), Eq(std::chrono::minutes(0) + std::chrono::seconds(12)));
	EXPECT_THAT(RunTime::toDuration(RunTimeDto(1, 25)), Eq(std::chrono::minutes(1) + std::chrono::seconds(25)));
}
