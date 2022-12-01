#include "Exceptions/Exceptions.h"
#include "Logic/RunTimeImpl.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeImplTest, defaultConstructor) {
	RunTimeImpl runTime;
	EXPECT_THAT(runTime.getSeconds(), Eq(0));
}

TEST(RunTimeImplTest, parametrizedConstructor) {
	RunTimeImpl runTime(10);
	EXPECT_THAT(runTime.getSeconds(), Eq(10));
}

TEST(RunTimeImplTest, setValue) {
	RunTimeImpl runTime;

	runTime.setSeconds(25);
	EXPECT_THAT(runTime.getSeconds(), Eq(25));

	runTime.setSeconds(38);
	EXPECT_THAT(runTime.getSeconds(), Eq(38));
}

TEST(RunTimeImplTest, setValueMax) {
	EXPECT_NO_THROW(RunTimeImpl().setSeconds(24 * 60 * 60));
	EXPECT_THROW(RunTimeImpl().setSeconds(24 * 60 * 60 + 1), ValueOutOfBoundsException);
}

TEST(RunTimeImplTest, toDuration) {
	EXPECT_THAT(RunTimeImpl(0).toDuration(), Eq(std::chrono::seconds(0)));
	EXPECT_THAT(RunTimeImpl(12).toDuration(), Eq(std::chrono::seconds(12)));
	EXPECT_THAT(RunTimeImpl(85).toDuration(), Eq(std::chrono::seconds(85)));
}

TEST(RunTimeImplTest, toDurationStatic) {
	EXPECT_THAT(RunTime::toDuration(RunTimeDTO(0, 0)), Eq(std::chrono::seconds(0)));
	EXPECT_THAT(RunTime::toDuration(RunTimeDTO(0, 12)), Eq(std::chrono::seconds(12)));
	EXPECT_THAT(RunTime::toDuration(RunTimeDTO(1, 25)), Eq(std::chrono::seconds(85)));
}
