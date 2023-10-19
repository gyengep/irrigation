#include "RunTimeImplTest.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeImplConstructorTest, defaultConstructor) {
	RunTimeImpl runTime;
	EXPECT_THAT(runTime.getSeconds(), Eq(0));
}

TEST(RunTimeImplConstructorTest, parametrizedConstructor) {
	RunTimeImpl runTime(10);
	EXPECT_THAT(runTime.getSeconds(), Eq(10));
}

///////////////////////////////////////////////////////////////////////////////

void RunTimeImplTest::SetUp() {
	runTime = std::make_shared<RunTimeImpl>();
}

void RunTimeImplTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeImplTest, setValue) {
	runTime->setSeconds(25);
	EXPECT_THAT(runTime->getSeconds(), Eq(25));

	runTime->setSeconds(38);
	EXPECT_THAT(runTime->getSeconds(), Eq(38));
}

TEST_F(RunTimeImplTest, setValueMax) {
	EXPECT_NO_THROW(RunTimeImpl().setSeconds(24 * 60 * 60));
	EXPECT_THROW(RunTimeImpl().setSeconds(24 * 60 * 60 + 1), ValueOutOfBoundsException);
}

TEST_F(RunTimeImplTest, toDuration) {
	EXPECT_THAT(RunTimeImpl(0).toDuration(), Eq(std::chrono::seconds(0)));
	EXPECT_THAT(RunTimeImpl(12).toDuration(), Eq(std::chrono::seconds(12)));
	EXPECT_THAT(RunTimeImpl(85).toDuration(), Eq(std::chrono::seconds(85)));
}

TEST_F(RunTimeImplTest, toDurationStatic) {
	EXPECT_THAT(RunTime::toDuration(RunTimeDTO(0, 0)), Eq(std::chrono::seconds(0)));
	EXPECT_THAT(RunTime::toDuration(RunTimeDTO(0, 12)), Eq(std::chrono::seconds(12)));
	EXPECT_THAT(RunTime::toDuration(RunTimeDTO(1, 25)), Eq(std::chrono::seconds(85)));
}
